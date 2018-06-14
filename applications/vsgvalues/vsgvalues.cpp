#include <vsg/core/Object.h>
#include <vsg/core/Auxiliary.h>
#include <vsg/core/Value.h>
#include <vsg/core/Visitor.h>

#include <iostream>
#include <typeinfo>

// helper function to simplify iteration through any user objects/values assigned to an object.
template<typename P, typename F>
void for_each_user_object(P object, F functor)
{
    if (object->getAuxiliary())
    {
        using ObjectMap = vsg::Auxiliary::ObjectMap;
        ObjectMap& objectMap = object->getAuxiliary()->getObjectMap();
        for (ObjectMap::iterator itr = objectMap.begin();
             itr != objectMap.end();
             ++itr)
        {
            functor(*itr);
        }
    }
}

int main(int argc, char** argv)
{
    vsg::ref_ptr<vsg::Object> object = new vsg::Object;
    object->setValue("name", "Name field contents");
    object->setValue("time", 10.0);
    object->setValue("size", 3.1f);
    object->setValue("count", 5);
    object->setValue("pos", 4u);

    if (object->getAuxiliary())
    {
        struct VisitValues : public vsg::Visitor
        {
            void apply(vsg::Object& object)
            {
                std::cout<<"Object, "<<typeid(object).name()<<std::endl;
            }

            void apply(vsg::IntValue& value)
            {
                std::cout<<"IntValue,  value = "<<value.value<<std::endl;
            }

            void apply(vsg::UIntValue& value)
            {
                std::cout<<"UIntValue,  value = "<<value.value<<std::endl;
            }

            void apply(vsg::FloatValue& value)
            {
                std::cout<<"FloatValue, value  = "<<value.value<<std::endl;
            }

            void apply(vsg::DoubleValue& value)
            {
                std::cout<<"DoubleValue, value  = "<<value.value<<std::endl;
            }

            void apply(vsg::StringValue& value)
            {
                std::cout<<"StringValue, value  = "<<value.value<<std::endl;
            }
        };

        VisitValues visitValues;

        std::cout<<"Object has Auxiliary so check it's ObjectMap for our values. "<<object->getAuxiliary()<<std::endl;
        for(vsg::Auxiliary::ObjectMap::iterator itr = object->getAuxiliary()->getObjectMap().begin();
            itr != object->getAuxiliary()->getObjectMap().end();
            ++itr)
        {
            std::cout<<"   key["<<itr->first.name<<", "<<itr->first.index<<"] ";
            itr->second->accept(visitValues);
        }


        std::cout<<"Use for_each "<<std::endl;
        std::for_each(object->getAuxiliary()->getObjectMap().begin(), object->getAuxiliary()->getObjectMap().end(), [&visitValues](vsg::Auxiliary::ObjectMap::value_type& key_value)
        {
            std::cout<<"   key["<<key_value.first.name<<", "<<key_value.first.index<<"] ";
            key_value.second->accept(visitValues);
        });

        std::cout<<"for_each_user_object "<<std::endl;
        for_each_user_object(object, [&visitValues](vsg::Auxiliary::ObjectMap::value_type& key_value)
        {
            std::cout<<"   key["<<key_value.first.name<<", "<<key_value.first.index<<"] ";
            key_value.second->accept(visitValues);
        });

    }



    return 0;
}