#include "icp.h"
#include "pcl-1.12/pcl/kdtree/kdtree_flann.h"

namespace ns_section {
  std::pair<ns_geo::PointSet3d, ns_geo::Point3d> ICP::normalize(const ns_geo::PointSet3d &pc) {
    ns_geo::Point3d cen(0.0, 0.0, 0.0);
    for (int i = 0; i != pc.size(); ++i) {
      const auto &p = pc[i];
      cen.x += p.x, cen.y += p.y, cen.z += p.z;
    }
    cen.x /= pc.size(), cen.y /= pc.size(), cen.z /= pc.size();
    ns_geo::PointSet3d normalized(pc.size());
    for (int i = 0; i != pc.size(); ++i) {
      const auto &p = pc[i];
      normalized[i].x = p.x - cen.x;
      normalized[i].y = p.y - cen.y;
      normalized[i].z = p.z - cen.z;
    }
    return {normalized, cen};
  }

  ns_geo::Point3d ICP::nearest(const ns_geo::PointSet3d &pc, const ns_geo::Point3d &p) {
    auto iter = std::min_element(
        pc.cbegin(), pc.cend(), [p](const ns_geo::Point3d &p1, const ns_geo::Point3d &p2) {
          return ((p.x - p1.x) * (p.x - p1.x) + (p.y - p1.y) * (p.y - p1.y) + (p.z - p1.z) * (p.z - p1.z)) <
                 ((p.x - p2.x) * (p.x - p2.x) + (p.y - p2.y) * (p.y - p2.y) + (p.z - p2.z) * (p.z - p2.z));
        });
    return *iter;
  }

  Sophus::SE3d ICP::solve(const ns_geo::PointSet3d &pointCloud1, const ns_geo::PointSet3d &pointCloud2) {
    // normalize the point clouds
    auto [normPointCloud1, center1_pt] = ICP::normalize(pointCloud1);
    auto [normPointCloud2, center2_pt] = ICP::normalize(pointCloud2);

    // kdtree
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
    cloud->resize(normPointCloud2.size());
    for (int i = 0; i != normPointCloud2.size(); ++i) {
      (*cloud)[i] = pcl::PointXYZ(normPointCloud2[i].x, normPointCloud2[i].y, normPointCloud2[i].z);
    }
    pcl::KdTreeFLANN<pcl::PointXYZ> kdtree;
    kdtree.setInputCloud(cloud);

    // compute rotation
    Sophus::SO3d R21;
    while (true) {
      Eigen::Matrix3d HMat = Eigen::Matrix3d::Zero();
      Eigen::Vector3d gVec = Eigen::Vector3d::Zero();
      for (int j = 0; j != normPointCloud1.size(); ++j) {
        const auto &np1_pt = normPointCloud1[j];

        // trans
        Eigen::Vector3d prime = R21 * ICP::toVec3d(np1_pt);
        // find nearest
        pcl::PointXYZ searchPoint(prime(0), prime(1), prime(2));
        std::vector<int> pointIdxKNNSearch(1);
        std::vector<float> pointKNNSquaredDistance(1);
        int state = kdtree.nearestKSearch(searchPoint, 1, pointIdxKNNSearch, pointKNNSquaredDistance);
        pcl::PointXYZ target = (*cloud)[pointIdxKNNSearch[0]];

        ns_geo::Point3d np2_pt(target.x, target.y, target.z);

        // compute error
        Eigen::Vector3d error = prime - ICP::toVec3d(np2_pt);
        // compute jacobi
        Eigen::Matrix3d jacobi = -Sophus::SO3d::hat(prime);
        HMat += jacobi.transpose() * jacobi;
        gVec -= jacobi.transpose() * error;
      }
      // solve
      Eigen::Vector3d delta = HMat.ldlt().solve(gVec);
      // update
      R21 = Sophus::SO3d::exp(delta) * R21;
      LOG_VAR(delta.norm());
      if (delta.norm() < 1E-10) {
        break;
      }
    }

    // compute translation
    Eigen::Vector3d cen1 = ICP::toVec3d(center1_pt);
    Eigen::Vector3d cen2 = ICP::toVec3d(center2_pt);
    Eigen::Vector3d t21 = cen2 - R21 * cen1;

    return Sophus::SE3d(R21.matrix(), t21);
  }

  Eigen::Vector3d ICP::toVec3d(const ns_geo::Point3d &p) {
    return Eigen::Vector3d(p.x, p.y, p.z);
  }

  ns_geo::Point3d ICP::fromVec3d(const Eigen::Vector3d &vec) {
    return ns_geo::Point3d(vec(0), vec(1), vec(2));
  }
} // namespace ns_section