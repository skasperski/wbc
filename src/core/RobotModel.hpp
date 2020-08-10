#ifndef ROBOTMODEL_HPP
#define ROBOTMODEL_HPP

#include <vector>
#include <base/Eigen.hpp>
#include <map>
#include <base/samples/Joints.hpp>
#include <base/samples/RigidBodyStateSE3Vector.hpp>
#include <memory>
#include "RobotModelConfig.hpp"
#include <base/JointLimits.hpp>

namespace wbc{

/**
 * @brief Interface for all robot models. This has to provide all kinematics and dynamics information that is required for WBC
 */
class RobotModel{

protected:
    std::vector<std::string> actuated_joint_names;
    std::vector<std::string> all_joint_names;
    base::NamedVector<base::samples::RigidBodyStateSE3> robot_models_state;
    std::string base_frame;
    base::Time last_update;
    base::JointLimits joint_limits;

public:
    RobotModel();
    virtual ~RobotModel();

    /**
     * @brief Load and configure the robot model with single model file
     * @param model_file The models configuration file.
     * @param joint_names Order of joint names within the model.
     * @param base_frame Base frame of the model.
     * @return True in case of success, else false
     */
    virtual bool configure(const std::string& model_filename,
                           const std::vector<std::string> &joint_names = std::vector<std::string>(),
                           const std::string &base_frame = "") = 0;

    /**
     * @brief Load and configure the robot model
     * @param model_config The models configuration(s). These include the path to the robot model file(s), the relative poses and hooks
     *                     to which the models shall be attached. This way you can add multiple robot model tree and attach them to each other.
     * @param joint_names Order of joint names within the model.
     * @param base_frame Base frame of the model.
     * @return True in case of success, else false
     */
    virtual bool configure(const std::vector<RobotModelConfig>& model_config,
                           const std::vector<std::string> &joint_names = std::vector<std::string>(),
                           const std::string &base_frame = "") = 0;

    /**
     * @brief Update the robot model
     * @param joint_state The joint_state vector. Has to contain all robot joints.
     * @param poses Optionally update links of the robot model. This can be used to update e.g. the relative position between two robots in the model.
     */
    virtual void update(const base::samples::Joints& joint_state,
                        const base::NamedVector<base::samples::RigidBodyStateSE3>& virtual_joint_states = base::NamedVector<base::samples::RigidBodyStateSE3>()) = 0;

    /** Returns the relative transform between the two given frames. By convention this is the pose of the tip frame in root coordinates!*/
    virtual const base::samples::RigidBodyStateSE3 &rigidBodyState(const std::string &root_frame, const std::string &tip_frame) = 0;

    /** Returns the current status of the given joint names */
    virtual const base::samples::Joints& jointState(const std::vector<std::string> &joint_names) = 0;

    /** Returns the Jacobian for the kinematic chain between root and the tip frame. By convention reference frame & reference point
      *  of the Jacobian will be the root frame. Size of the Jacobian will be 6 x nJoints, where nJoints is the number of joints in the kinematic chain.*/
    virtual const base::MatrixXd &jacobian(const std::string &root_frame, const std::string &tip_frame) = 0;

    /** @brief Returns the Jacobian for the kinematic chain between root and the tip frame as full body Jacobian. By convention reference frame & reference point
      *  of the Jacobian will be the root frame. Size of the Jacobian will be 6 x nJoints, where nJoints is the number of joints of the whole robot. The order of the
      * columns will be the same as the joint order of the robot. The columns that correspond to joints that are not part of the kinematic chain will have only zeros as entries.
      * @param root_frame Root frame of the chain. Has to be a valid link in the robot model.
      * @param tip_frame Tip frame of the chain. Has to be a valid link in the robot model.
      */
    virtual const base::MatrixXd &fullJacobian(const std::string &root_frame, const std::string &tip_frame) = 0;

    /** Returns the derivative of the Jacobian for the kinematic chain between root and the tip frame. By convention the Jacobian is computed with respect to
        the root frame with the rotation point at the tip frame*/
    virtual const base::MatrixXd &jacobianDot(const std::string &root_frame, const std::string &tip_frame) = 0;

    /** @brief Returns the derivative of the Jacobian for the kinematic chain between root and the tip frame as full body Jacobian. By convention reference frame & reference point
      *  of the Jacobian will be the root frame. Size of the Jacobian will be 6 x nJoints, where nJoints is the number of joints of the whole robot. The order of the
      * columns will be the same as the joint order of the robot. The columns that correspond to joints that are not part of the kinematic chain will have only zeros as entries.
      * @param root_frame Root frame of the chain. Has to be a valid link in the robot model.
      * @param tip_frame Tip frame of the chain. Has to be a valid link in the robot model.
      * @return A 6xN Jacobian derivative matrix, where N is the number of robot joints
      */
    virtual const base::MatrixXd &fullJacobianDot(const std::string &root_frame, const std::string &tip_frame) = 0;

    /** Return the number of actuated joints in the robot model*/
    uint noOfActuatedJoints(){return actuated_joint_names.size();}

    /** Return the overall number of joints in the robot model*/
    uint noOfJoints(){return all_joint_names.size();}

    /** Return all joint names*/
    const std::vector<std::string>& jointNames(){return all_joint_names;}

    /** Return only actuated joint names*/
    const std::vector<std::string>& actuatedJointNames(){return actuated_joint_names;}

    /** Get index of joint name*/
    uint jointIndex(const std::string &joint_name);

    /** Get the base frame of the robot*/
    const std::string& baseFrame();

    /** Get time timestamp of the update */
    base::Time lastUpdate(){return last_update;}

    /** Get the names of all robot models*/
    const base::NamedVector<base::samples::RigidBodyStateSE3>& robotModelsState(){return robot_models_state;}

    /** Return current joint limits*/
    const base::JointLimits& jointLimits(){return joint_limits;}

};

typedef std::shared_ptr<RobotModel> RobotModelPtr;

}

#endif // ROBOTMODEL_HPP
