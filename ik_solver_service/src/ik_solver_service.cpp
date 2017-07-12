// This file is part of RPG-YTC- the RPG youBot Torque Controller
//
// RPG-YTC is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// RPG-YTC is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with RPG-YTC.  If not, see <http://www.gnu.org/licenses/>.

#include "ik_solver_service/youbot_grasp_ik.h"

#include "ik_solver_service/SolveClosestIK.h"
#include "ik_solver_service/SolvePreferredPitchIK.h"
#include "ik_solver_service/SolvePreferredTypeIK.h"
#include "ik_solver_service/SolveFullyConstrainedIK.h"
#include "ik_solver_service/SolveFullyConstrainedIKArray.h"
#include "ik_solver_service/FullyConstrainedReq.h"
#include "ik_solver_service/FullyConstrainedRes.h"

namespace ik_solver_service {

double YoubotGraspIK::lox_ = 0.033;
double YoubotGraspIK::loz_ = 0.1472;
double YoubotGraspIK::l_2_ = 0.155;
double YoubotGraspIK::l_3_ = 0.135;
double YoubotGraspIK::l_4_ = 0.218;

bool solveClosestIK(ik_solver_service::SolveClosestIK::Request &req, ik_solver_service::SolveClosestIK::Response &res)
{
  joint_positions_solution_t current_joint_positions;
  geometry_msgs::Point desired_position;
  geometry_msgs::Vector3 desired_normal;

  for (int i = 0; i < 5; i++)
  {
    current_joint_positions.joints[i] = req.joint_angles[i];
  }
  desired_position.x = req.des_position[0];
  desired_position.y = req.des_position[1];
  desired_position.z = req.des_position[2];
  desired_normal.x = req.des_normal[0];
  desired_normal.y = req.des_normal[1];
  desired_normal.z = req.des_normal[2];

  joint_positions_solution_t closest_solution = YoubotGraspIK::solveClosestIK(current_joint_positions, desired_position,
                                                                              desired_normal);

  for (int i = 0; i < 5; i++)
  {
    res.joint_angles[i] = closest_solution.joints[i];
  }
  res.feasible = closest_solution.feasible;
  res.arm_to_front = closest_solution.arm_to_front;
  res.arm_bended_up = closest_solution.arm_bended_up;
  res.gripper_downwards = closest_solution.gripper_downwards;

  return true;
}

bool solvePreferredPitchIK(ik_solver_service::SolvePreferredPitchIK::Request &req,
                           ik_solver_service::SolvePreferredPitchIK::Response &res)
{
  geometry_msgs::Point desired_position;
  geometry_msgs::Vector3 desired_normal;

  desired_position.x = req.des_position[0];
  desired_position.y = req.des_position[1];
  desired_position.z = req.des_position[2];
  desired_normal.x = req.des_normal[0];
  desired_normal.y = req.des_normal[1];
  desired_normal.z = req.des_normal[2];

  joint_positions_solution_t preferred_pitch_solution = YoubotGraspIK::solvePreferredPitchIK(req.preferred_pitch,
                                                                                             desired_position,
                                                                                             desired_normal);
  for (int i = 0; i < 5; i++)
  {
    res.joint_angles[i] = preferred_pitch_solution.joints[i];
  }
  res.feasible = preferred_pitch_solution.feasible;
  res.arm_to_front = preferred_pitch_solution.arm_to_front;
  res.arm_bended_up = preferred_pitch_solution.arm_bended_up;
  res.gripper_downwards = preferred_pitch_solution.gripper_downwards;

  return true;
}

bool solvePreferredTypeIK(ik_solver_service::SolvePreferredTypeIK::Request &req,
                          ik_solver_service::SolvePreferredTypeIK::Response &res)
{
  geometry_msgs::Point desired_position;
  geometry_msgs::Vector3 desired_normal;

  desired_position.x = req.des_position[0];
  desired_position.y = req.des_position[1];
  desired_position.z = req.des_position[2];
  desired_normal.x = req.des_normal[0];
  desired_normal.y = req.des_normal[1];
  desired_normal.z = req.des_normal[2];

  joint_positions_solution_t preferred_type_solution = YoubotGraspIK::solvePreferredTypeIK(req.arm_to_front,
                                                                                           req.arm_bended_up,
                                                                                           req.gripper_downwards,
                                                                                           desired_position,
                                                                                           desired_normal);

  for (int i = 0; i < 5; i++)
  {
    res.joint_angles[i] = preferred_type_solution.joints[i];
  }
  res.feasible = preferred_type_solution.feasible;
  res.arm_to_front = preferred_type_solution.arm_to_front;
  res.arm_bended_up = preferred_type_solution.arm_bended_up;
  res.gripper_downwards = preferred_type_solution.gripper_downwards;

  return true;
}

bool solveFullyConstrainedIK(ik_solver_service::SolveFullyConstrainedIK::Request &req,
                             ik_solver_service::SolveFullyConstrainedIK::Response &res)
{
  int id;
  double pitch;
  geometry_msgs::Point desired_position;
  geometry_msgs::Vector3 desired_normal;

  id = req.id;
  pitch = req.pitch;
  desired_position.x = req.des_position[0];
  desired_position.y = req.des_position[1];
  desired_position.z = req.des_position[2];
  desired_normal.x = req.des_normal[0];
  desired_normal.y = req.des_normal[1];
  desired_normal.z = req.des_normal[2];

  joint_positions_solution_t fully_constrained_solution = YoubotGraspIK::solveFullyConstrainedIK(id, pitch,
                                                                                                 desired_position,
                                                                                                 desired_normal);

  for (int i = 0; i < 5; i++)
  {
    res.joint_angles[i] = fully_constrained_solution.joints[i];
  }
  res.feasible = fully_constrained_solution.feasible;
  res.arm_to_front = fully_constrained_solution.arm_to_front;
  res.arm_bended_up = fully_constrained_solution.arm_bended_up;
  res.gripper_downwards = fully_constrained_solution.gripper_downwards;

  return true;
}

bool solveFullyConstrainedIKArray(ik_solver_service::SolveFullyConstrainedIKArray::Request &req,
                                  ik_solver_service::SolveFullyConstrainedIKArray::Response &res)
{
  ik_solver_service::FullyConstrainedReq request;
  ik_solver_service::FullyConstrainedRes response;
  while (!req.ikarray.empty())
  {
    request = req.ikarray.back();
    req.ikarray.pop_back();
    int id;
    double pitch;
    geometry_msgs::Point desired_position;
    geometry_msgs::Vector3 desired_normal;

    id = request.id;
    pitch = request.pitch;
    desired_position.x = request.des_position[0];
    desired_position.y = request.des_position[1];
    desired_position.z = request.des_position[2];
    desired_normal.x = request.des_normal[0];
    desired_normal.y = request.des_normal[1];
    desired_normal.z = request.des_normal[2];

    joint_positions_solution_t fully_constrained_solution = YoubotGraspIK::solveFullyConstrainedIK(id, pitch,
                                                                                                   desired_position,
                                                                                                   desired_normal);

    for (int i = 0; i < 5; i++)
    {
      response.joint_angles[i] = fully_constrained_solution.joints[i];
    }
    response.feasible = fully_constrained_solution.feasible;
    response.arm_to_front = fully_constrained_solution.arm_to_front;
    response.arm_bended_up = fully_constrained_solution.arm_bended_up;
    response.gripper_downwards = fully_constrained_solution.gripper_downwards;
    res.ikresp.insert(res.ikresp.begin(), response);
  }
  return true;
}

} // namespace ik_solver_service

using namespace ik_solver_service;

int main(int argc, char **argv)
{
  ros::init(argc, argv, "solve_ik_server");
  ros::NodeHandle n("~");

  double param;
  if (n.getParam("lox", param)){
    YoubotGraspIK::lox_ = param;
  } 
  if (n.getParam("loz", param)){
    YoubotGraspIK::loz_ = param;
  } 
  if (n.getParam("l2", param)){
    YoubotGraspIK::l_2_ = param;
  } 
  if (n.getParam("l3", param)){
    YoubotGraspIK::l_3_ = param;
  } 
  if (n.getParam("l4", param)){
    YoubotGraspIK::l_4_ = param;
  } 

  ROS_INFO_STREAM("IK params - Lox: " << YoubotGraspIK::lox_ << " Loz: " << YoubotGraspIK::loz_ <<" L2: " << YoubotGraspIK::l_2_ << " L3: " << YoubotGraspIK::l_3_ << " L4: " << YoubotGraspIK::l_4_);

  ros::ServiceServer solve_closest_ik_service = n.advertiseService("solve_closest_ik", solveClosestIK);
  ros::ServiceServer solve_preferred_pitch_ik_service = n.advertiseService("solve_preferred_pitch_ik",
                                                                           solvePreferredPitchIK);
  ros::ServiceServer solve_preferred_type_ik_service = n.advertiseService("solve_preferred_type_ik",
                                                                          solvePreferredTypeIK);
  ros::ServiceServer solve_fully_constrained_ik_service = n.advertiseService("solve_fully_constrained_ik",
                                                                             solveFullyConstrainedIK);
  ros::ServiceServer solve_fully_constrained_ik_service_array = n.advertiseService("solve_fully_constrained_ik_array",
                                                                                   solveFullyConstrainedIKArray);

  ros::spin();

  return 0;
}

