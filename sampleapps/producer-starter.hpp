#ifndef NDNSIM_EXAMPLES_NDN_CXX_SIMPLE_CUSTOM_PRODUCER_STARTER_HPP
#define NDNSIM_EXAMPLES_NDN_CXX_SIMPLE_CUSTOM_PRODUCER_STARTER_HPP

#include "producer.hpp"

#include "ns3/ndnSIM/helper/ndn-stack-helper.hpp"
#include "ns3/application.h"

namespace ns3 {

// Class inheriting from ns3::Application
class CustomProducerStarter : public Application
{
public:
  static TypeId
  GetTypeId()
  {
    static TypeId tid = TypeId("CustomProducerStarter")
      .SetParent<Application>()
      .AddConstructor<CustomProducerStarter>();

    return tid;
  }

protected:
  // inherited from Application base class.
  virtual void
  StartApplication()
  {
    // Create an instance of the app, and passing the dummy version of KeyChain (no real signing)
    m_instance.reset(new app::CustomProducer(ndn::StackHelper::getKeyChain()));
    m_instance->run(); // can be omitted
  }

  virtual void
  StopApplication()
  {
    // Stop and destroy the instance of the app
    m_instance.reset();
  }

private:
  std::unique_ptr<app::CustomProducer> m_instance;
};

} // namespace ns3

#endif // NDNSIM_EXAMPLES_NDN_CXX_SIMPLE_REAL_APP_STARTER_HPP