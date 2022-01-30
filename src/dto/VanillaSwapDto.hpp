#ifndef VanillaSwapDto_hpp
#define VanillaSwapDto_hpp

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"
#include <ql/instruments/vanillaswap.hpp>
#include <ql/time/frequency.hpp>
#include <ql/time/daycounters/actual360.hpp>
#include <ql/time/daycounters/thirty360.hpp>
#include <ql/time/daycounters/actual365fixed.hpp>
#include <ql/currencies/asia.hpp>
#include <ql/currencies/america.hpp> 
#include <ql/currencies/europe.hpp>
#include <ql/indexes/ibor/all.hpp>


#include OATPP_CODEGEN_BEGIN(DTO)

ENUM(SwapType, v_int32,
     VALUE(PAYER, QuantLib::Swap::Payer, "PAYER"),
     VALUE(RECIVER, QuantLib::Swap::Receiver, "RECIEVER")
);

ENUM(PaymentFrequency, v_int32,
     VALUE(ANNUAL, QuantLib::Annual, "ANNUAL"),
     VALUE(SEMIANNUAL, QuantLib::Semiannual, "SEMIANNUAL"),
     VALUE(QUARTERLY, QuantLib::Quarterly, "QUARTERLY"),
     VALUE(MONTHLY, QuantLib::Monthly, "MONTHLY"),
     VALUE(WEEKLY, QuantLib::Weekly, "WEEKLY"),
     VALUE(DAILY, QuantLib::Daily, "DAILY")
);

ENUM(CurrencyType, v_int32,
     VALUE(USD,840,"USD"),
     VALUE(JPY,392,"JPY"),
     VALUE(EUR,978,"EUR"),
     VALUE(GBP,826,"GBP")
);

ENUM(DaycountType, v_int32,
     VALUE(THIRTY360, 1, "THIRTY360"),
     VALUE(ACTUAL360, 2, "ACTUAL360"),
     VALUE(ACTUAL365, 3, "ACTUAL365")
);
ENUM(IndexFamily, v_int32,
     VALUE(LIBOR,1, "LBOR"),
     VALUE(OIS,2,"OIS")
);

class VanillaSwapDto : public oatpp::DTO {
  
     DTO_INIT(VanillaSwapDto, DTO);

     DTO_FIELD(Int32, id);         // id 
     DTO_FIELD(Float32, notional);  // notional
     DTO_FIELD(Enum<SwapType>::AsNumber, type, "type");
     DTO_FIELD(Int64, startDate);  // milliseconds since 1970
     DTO_FIELD(Int64, mtyDate);    // milliseconds since 1970
     DTO_FIELD(Float32, fixedRate); // percent
     DTO_FIELD(Enum<CurrencyType>::AsNumber, fixedCurrency, "fixedCurrency");
     DTO_FIELD(Enum<PaymentFrequency>::AsNumber, fixedFrequency, "fixedFrequency");
     DTO_FIELD(Enum<DaycountType>::AsNumber, fixedDaycount, "fixedDaycount");
     DTO_FIELD(Float32, floatSpread);  // percent
     DTO_FIELD(Enum<IndexFamily>::AsNumber, floatIndexFamily);
     DTO_FIELD(Enum<CurrencyType>::AsNumber, floatCurrency, "floatCurrency");
     DTO_FIELD(Enum<PaymentFrequency>::AsNumber, floatFrequency, "floatFrequency");
     DTO_FIELD(Enum<PaymentFrequency>::AsNumber, floatTenor, "floatTenor");
     DTO_FIELD(Enum<DaycountType>::AsNumber, floatDaycount, "floatDaycount");
     
     QuantLib::VanillaSwap::Type getType () {
          SwapType swapType = type;
          switch (swapType) {
               case SwapType::PAYER:
                    return QuantLib::VanillaSwap::Payer;
               case SwapType::RECIVER:
                    return QuantLib::VanillaSwap::Receiver;
               default:
                    break;
          }
          throw ("Vanilla Swap - Undefined type");
     }
     QuantLib::Currency getFixedCurrency () {return getCurrency (fixedCurrency);}
     QuantLib::Currency getCurrency (CurrencyType currency) {
          switch (currency) {
               case CurrencyType::USD:
                    return QuantLib::USDCurrency ();
               case CurrencyType::EUR:
                    return QuantLib::EURCurrency ();
               case CurrencyType::JPY:
                    return QuantLib::JPYCurrency ();
               case CurrencyType::GBP:
                    return QuantLib::GBPCurrency ();
               default:
                    break;
          }
          throw ("Vanilla Swap - Undefined Currency");
     }
     std::string getFamiilyName () {
          IndexFamily family = floatIndexFamily;
          switch (family) {
               case IndexFamily::LIBOR:
                    return "Libor";
               case IndexFamily::OIS:
                    return "OIS";
               default: 
                    break;
          }
          throw ("Vanilla Swap - Undefined Floating Index Family Name");
     }
     QuantLib::Frequency getFixedFrequency () {return getFrequency (fixedFrequency);}
     QuantLib::Frequency getFloatFrequency () {return getFrequency (floatFrequency);}
     QuantLib::Frequency getFrequency (PaymentFrequency frequency) {
          switch (frequency) {
               case PaymentFrequency::ANNUAL:
                    return QuantLib::Annual;
               case PaymentFrequency::SEMIANNUAL:
                    return QuantLib::Semiannual;
               case PaymentFrequency::QUARTERLY:
                    return QuantLib::Quarterly;
               case PaymentFrequency::MONTHLY:
                    return QuantLib::Monthly;
               case PaymentFrequency::WEEKLY:
                    return QuantLib::Weekly;
               default:
                    break;
          }
          return QuantLib::NoFrequency;
     }
     QuantLib::Period getFloatTenor () {return QuantLib::Period (getFrequency (floatTenor));}
     QuantLib::ext::shared_ptr<QuantLib::IborIndex> getFloatingRateIndex (QuantLib::Handle<QuantLib::YieldTermStructure>& h) {
          CurrencyType currency = floatCurrency;
          IndexFamily  family   = floatIndexFamily;
          if (family == IndexFamily::LIBOR) {
		     switch (currency) {
                    case CurrencyType::USD:
                         return QuantLib::ext::shared_ptr<QuantLib::IborIndex> (new QuantLib::USDLibor (getFloatTenor (), h));
                    case CurrencyType::EUR:
                         return QuantLib::ext::shared_ptr<QuantLib::IborIndex> (new QuantLib::EURLibor (getFloatTenor (), h));
                    case CurrencyType::JPY:
                         return QuantLib::ext::shared_ptr<QuantLib::IborIndex> (new QuantLib::JPYLibor (getFloatTenor (), h));
                    case CurrencyType::GBP:
                         return QuantLib::ext::shared_ptr<QuantLib::IborIndex> (new QuantLib::GBPLibor (getFloatTenor (), h));
                    default:
                         break;
               }
          }
          throw ("Vanilla Swap - Undefined Floating Rate Index");
     }
     QuantLib::ext::shared_ptr<QuantLib::DayCounter> getFixedDayCounter () {return getDayCounter (fixedDaycount);}
     QuantLib::ext::shared_ptr<QuantLib::DayCounter> getFloatDayCounter () {return getDayCounter (floatDaycount);}
     QuantLib::ext::shared_ptr<QuantLib::DayCounter> getDayCounter (DaycountType daycountType) {
          switch (daycountType) {
               case DaycountType::THIRTY360:
                    return QuantLib::ext::shared_ptr<QuantLib::DayCounter> (new QuantLib::Thirty360 (QuantLib::Thirty360::ISDA));
               case DaycountType::ACTUAL360:
                    return QuantLib::ext::shared_ptr<QuantLib::DayCounter> (new QuantLib::Actual360 ());
               case DaycountType::ACTUAL365:
                    return QuantLib::ext::shared_ptr<QuantLib::DayCounter> (new QuantLib::Actual365Fixed ());
               default: 
                    break;
          }
          throw ("Vanilla Swap - Undefined Day Count Type");
     }
};

#include OATPP_CODEGEN_END(DTO)

#endif /* VanillaSwapDto_hpp */
