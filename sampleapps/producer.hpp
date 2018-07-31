#ifndef NDNSIM_EXAMPLES_NDN_CXX_SIMPLE_CUSTOM_PRODUCER_HPP
#define NDNSIM_EXAMPLES_NDN_CXX_SIMPLE_CUSTOM_PRODUCER_HPP

#include <ndn-cxx/face.hpp>
#include <ndn-cxx/interest.hpp>
#include <ndn-cxx/security/key-chain.hpp>
#include <ndn-cxx/util/scheduler.hpp>

#include <iostream>

namespace app {

class CustomProducer
{
public:
  CustomProducer(ndn::KeyChain& keyChain)
    : m_keyChain(keyChain)
    // , m_faceProducer(m_face.getIoService())
    , m_scheduler(m_face.getIoService())
  {
    // // register prefix and set interest filter on producer face
    // m_faceProducer.setInterestFilter("/hello", std::bind(&RealApp::respondToAnyInterest, this, _2),
    //                                  std::bind([]{}), std::bind([]{}));

    // // use scheduler to send interest later on consumer face
    // m_scheduler.scheduleEvent(ndn::time::seconds(2), [this] {
    //     m_faceConsumer.expressInterest(ndn::Interest("/hello/world"),
    //                                    std::bind([] { std::cout << "Hello!" << std::endl; }),
    //                                    std::bind([] { std::cout << "NACK!" << std::endl; }),
    //                                    std::bind([] { std::cout << "Bye!.." << std::endl; }));
    //   });
  }

  void
  run()
  {
  	m_face.setInterestFilter("/example/testApp",
  														std::bind(&CustomProducer::onInterest, this, _1, _2),
  														ndn::RegisterPrefixSuccessCallback(),
  														std::bind(&CustomProducer::onRegisterFailed, this, _1, _2));

    m_face.processEvents(); // ok (will not block and do nothing)
    // m_faceConsumer.getIoService().run(); // will crash
  }

private:
	void
	onInterest(const ndn::InterestFilter& filter, const ndn::Interest& interest)
	{
		std::cout << "<< I: " << interest << std::endl;

		ndn::Name dataName(interest.getName());
		dataName
			.append("testApp")
			.appendVersion();

		static const std::string content = "hogehogeclub";

		std::shared_ptr<ndn::Data> data = std::make_shared<ndn::Data>();
		data->setName(dataName);
		data->setFreshnessPeriod(ndn::time::seconds(10));
		data->setContent(reinterpret_cast<const uint8_t*>(content.c_str()), content.size());

		m_keyChain.sign(*data);

		std::cout << ">> D: " << *data << std::endl;
		m_face.put(*data);
	}

	void
	onRegisterFailed(const ndn::Name& prefix, const std::string& reason)
	{
		std::cerr << "ERROR: Failed to register prefix \""
							<< prefix <<"\" in local hub's daemon (" << reason << ")"
							<< std::endl;

		m_face.shutdown();
	}

  // void
  // respondToAnyInterest(const ndn::Interest& interest)
  // {
  //   auto data = std::make_shared<ndn::Data>(interest.getName());
  //   m_keyChain.sign(*data);
  //   m_faceProducer.put(*data);
  // }

private:
  ndn::KeyChain& m_keyChain;
  ndn::Face m_face;
  // ndn::Face m_faceConsumer;
  // ndn::Face m_faceProducer;
  ndn::Scheduler m_scheduler;
};

} // namespace app

#endif // NDNSIM_EXAMPLES_NDN_CXX_SIMPLE_REAL_APP_HPP
