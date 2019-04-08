#ifndef WBCVELOCITYSCENE_HPP
#define WBCVELOCITYSCENE_HPP

#include "WbcScene.hpp"
#include "JointVelocityConstraint.hpp"
#include "CartesianVelocityConstraint.hpp"

namespace wbc{

typedef std::shared_ptr<CartesianVelocityConstraint> CartesianVelocityConstraintPtr;
typedef std::shared_ptr<JointVelocityConstraint> JointVelocityConstraintPtr;

class WbcVelocityScene : public WbcScene{
protected:
    base::VectorXd solver_output_vel, robot_vel;

    /**
     * @brief Create a constraint and add it to the WBC scene
     */
    virtual ConstraintPtr createConstraint(const ConstraintConfig &config);

public:
    WbcVelocityScene(RobotModelPtr robot_model) :
        WbcScene(robot_model){}
    virtual ~WbcVelocityScene(){
    }

    /**
     * @brief Update the wbc scene
     */
    virtual void update();

    /**
     * @brief Compute y and y_solution for each constraint. y_solution denotes the constraint velocity that can be achieved
     *  with the solution generated by the solver and y denotes the actual joint velocity achieved by the robot.
     *  Both values can be used to evaluate the performance of WBC
     */
    virtual const ConstraintsStatus &updateConstraintsStatus(const base::commands::Joints& solver_output, const base::samples::Joints& joint_state);
};

} // namespace wbc

#endif
