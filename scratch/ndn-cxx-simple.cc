#include "ns3/sampleapps/consumer.hpp"
#include "ns3/sampleapps/consumer-starter.hpp"

#include "ns3/sampleapps/producer.hpp"
#include "ns3/sampleapps/producer-starter.hpp"

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ndnSIM-module.h"
#include "ns3/point-to-point-module.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED(CustomConsumerStarter);
NS_OBJECT_ENSURE_REGISTERED(CustomProducerStarter);

int
main(int argc, char* argv[])
{
  CommandLine cmd;
  cmd.Parse(argc, argv);

  // Ptr<Node> node = CreateObject<Node>();
  NodeContainer nodes;
  nodes.Create(2);

  PointToPointHelper p2p;
  p2p.Install(nodes.Get(0), nodes.Get(1));

  ndn::StackHelper ndnHelper;
  ndnHelper.SetDefaultRoutes(true);
  ndnHelper.InstallAll();

  ndn::AppHelper consumerHelper("CustomConsumerStarter");
  consumerHelper.Install(nodes.Get(0))
    .Start(Seconds(6.5));

  ndn::AppHelper producerHelper("CustomProducerStarter");
  producerHelper.Install(nodes.Get(1))
    .Start(Seconds(6.5));

  Simulator::Stop(Seconds(20.0));

  Simulator::Run();
  Simulator::Destroy();

  return 0;
}

} // namespace ns3

int
main(int argc, char* argv[])
{
  return ns3::main(argc, argv);
}
