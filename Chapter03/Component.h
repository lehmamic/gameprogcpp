//
// Created by Michael Lehmann on 10.01.21.
//

#ifndef CHAPTER02_COMPONENT_H
#define CHAPTER02_COMPONENT_H


class Component {
public:
    // Constructor
    // (the lower the update order, the earlier the component updates
    Component(class Actor* owner, int updateOrder = 100);

    // Destructor
    virtual ~Component();

    // Update this component by delta time
    virtual void Update(float deltaTime);
    int GetUpdateOrder() const { return mUpdateOrder; }
protected:
    // Owning actor
    class Actor* mOwner;

    // Update order of component
    int mUpdateOrder;
};


#endif //CHAPTER02_COMPONENT_H
