#ifndef NDNSIM_EXAMPLES_NDN_CXX_SIMPLE_CUSTOM_CONSUMER_HPP
#define NDNSIM_EXAMPLES_NDN_CXX_SIMPLE_CUSTOM_CONSUMER_HPP

#include <ndn-cxx/face.hpp>
#include <ndn-cxx/interest.hpp>
#include <ndn-cxx/security/key-chain.hpp>
#include <ndn-cxx/util/scheduler.hpp>

#include <iostream>

namespace app {

class CustomConsumer
{
public:
  CustomConsumer(ndn::KeyChain& keyChain)
    : m_keyChain(keyChain)
    , m_scheduler(m_face.getIoService())
  {
  }

  void
  run()
  {
  	ndn::Interest interest(ndn::Name("/example/testApp/randomData"));
  	interest.setInterestLifetime(ndn::time::milliseconds(1000));
  	interest.setMustBeFresh(true);

  	m_face.expressInterest(interest,
                          std::bind([] { std::cout << "Hello!" << std::endl; }),
                          std::bind([] { std::cout << "NACK!" << std::endl; }),
                          std::bind([] { std::cout << "Bye!.." << std::endl; }));

  	std::cout << "Sending: " << interest << std::endl;

    m_face.processEvents(); // ok (will not block and do nothing)
    // m_faceConsumer.getIoService().run(); // will crash
  }

private:
	void
	onData(const ndn::Interest& interest, const ndn::Data& data)
	{
		std::cout << data << std::endl;
	}

	void
	onNack(const ndn::Interest& interest, const ndn::lp::Nack& nack)
	{
		std::cout << "received Nack with reason " << nack.getReason() << " for interest" << interest << std::endl;
	}

	void
	onTimeout(const ndn::Interest& interest)
	{
		std::cout << "Timeout " << interest << std::endl;
	}

private:
  ndn::KeyChain& m_keyChain;
  ndn::Face m_face;
  ndn::Scheduler m_scheduler;
};

} // namespace app

#endif // NDNSIM_EXAMPLES_NDN_CXX_SIMPLE_REAL_APP_HPP