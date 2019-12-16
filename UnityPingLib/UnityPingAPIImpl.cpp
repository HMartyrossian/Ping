#include "stdafx.h"

#include <stdexcept>      // std::out_of_range

#include "UnityPingAPIImpl.h"

std::mutex CUnityPingAPIImpl::destinationToPingMutex;
std::unordered_map<std::string, CUnityPing*> CUnityPingAPIImpl::destinationToPing;

bool CUnityPingAPIImpl::Ping(const char* destination) {
	bool result = false;

	std::string key(destination);
	if (key.length() != 0) {
		destinationToPingMutex.lock();
		try {
			if (destinationToPing.at(key) != nullptr) {
				result = true;
			}
		}
		catch (std::out_of_range&) {
			auto value = new CUnityPing(destination);
			if (value->Resovle()) {
				// IP address successfully resolved. Have to ping the host:
				value->Ping();
				// Have to add object to the hash table only when can resolve IP address:
				destinationToPing.insert({ key, value });
				result = true;
			}
		}

		destinationToPingMutex.unlock();
	}

	return result;
}

bool CUnityPingAPIImpl::Success(const char* destination) {
	bool result = false;

	std::string key(destination);
	if (key.length() != 0) {
		try {
			destinationToPingMutex.lock();
			auto value = destinationToPing.at(key);
			destinationToPingMutex.unlock();
			if (value != nullptr) {
				result = value->Success();
			}
		}
		catch (std::out_of_range&) {
		}
	}

	return result;
}

long long CUnityPingAPIImpl::Time(const char* destination) {
	long long result = 0;

	std::string key(destination);
	if (key.length() != 0) {
		try {
			destinationToPingMutex.lock();
			auto value = destinationToPing.at(key);
			destinationToPingMutex.unlock();
			if (value != nullptr) {
				result = value->Time();

				// Working with row pointers in containers is more efficient that using shared_ptr<T>
				delete value;
			}

			// Have to remove CUnityPing object from the hash table.
			// Can use "Time Based Expiry" caching strategy to improve performance:
			destinationToPingMutex.lock();
			destinationToPing.erase(key);
			destinationToPingMutex.unlock();
		}
		catch (std::out_of_range& ex) {
			// Have to re-throw exception because of the protocol violation:
			throw ex;
		}
	}

	return result;
}
