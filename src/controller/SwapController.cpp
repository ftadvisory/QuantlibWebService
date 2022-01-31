#include "SwapController.hpp"

#include <ql/qldefines.hpp>
#if !defined(BOOST_ALL_NO_LIB) && defined(BOOST_MSVC)
#  include <ql/auto_link.hpp>
#endif

#include <ql/instruments/vanillaswap.hpp>
#include <ql/termstructures/yieldtermstructure.hpp>
#include <ql/time/calendars/target.hpp>
#include <ql/time/daycounters/simpledaycounter.hpp>
#include <ql/termstructures/yield/flatforward.hpp>
#include <ql/quotes/simplequote.hpp>
#include <ql/time/date.hpp>

#include <iostream>

using namespace QuantLib;

#if defined(QL_ENABLE_SESSIONS)
namespace QuantLib {

    ThreadKey sessionId() { return {}; }

}
#endif

void SwapController::createSwap (Object<VanillaSwapDto> dto) {
	try {
        ext::shared_ptr<VanillaSwap> swap = dto->getSwap (TARGET());
		std::ostringstream swapDetails;
		Date startDate = Date (dto->startDate);
		Date mtyDate   = Date (dto->mtyDate);
		swapDetails << "notional: " << swap->nominal () << " start date: " << startDate << " mty: " << mtyDate;
		OATPP_LOGI("SwapController::createSwap", "Swap Created %s", swapDetails.str ().c_str ());

	} catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "unknown error" << std::endl;
    }
}