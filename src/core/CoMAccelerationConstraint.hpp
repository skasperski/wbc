#ifndef COM_ACCELERATION_CONSTRAINT_HPP
#define COM_ACCELERATION_CONSTRAINT_HPP

#include "CartesianConstraint.hpp"

namespace wbc{

/**
 * @brief Implementation of a CoM velocity constraint.
 */
class CoMAccelerationConstraint : public CartesianConstraint{
public:
    CoMAccelerationConstraint(ConstraintConfig config, uint n_robot_joints);
    virtual ~CoMAccelerationConstraint();

    /**
     * @brief Update the CoM reference input for this constraint.
     * @param ref Reference input for this constraint. Only the velocity part is relevant (Must have a valid linear and angular velocity!)
     */
    virtual void setReference(const base::samples::RigidBodyStateSE3& ref);
};

typedef std::shared_ptr<CoMAccelerationConstraint> CoMAccelerationConstraintPtr;

} // namespace wbc

#endif
