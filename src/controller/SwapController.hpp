#ifndef SwapController_hpp
#define SwapController_hpp

#include "dto/DTOs.hpp"
#include "dto/VanillaSwapDto.hpp"

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"
#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen

/**
 * Sample Api Controller.
 */
class SwapController : public oatpp::web::server::api::ApiController {
public:
  /**
   * Constructor with object mapper.
   * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
   */
  SwapController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
    : oatpp::web::server::api::ApiController(objectMapper)
  {}
private:
  void createSwap (Object<VanillaSwapDto> dto);
public:
  
  ENDPOINT_INFO(root) {
    info->summary = "Quantlib Webservice";
    info->addResponse<Object<MyDto>>(
      Status::CODE_200, 
      "application/json");
  }
  ENDPOINT("GET", "/", root) {
    auto dto = MyDto::createShared();
    dto->statusCode = 200;
    dto->message = "Quantlib Webservice";
    return createDtoResponse(Status::CODE_200, dto);
  }
  ENDPOINT_INFO(echo) {
   info->summary = "Echo endpoint with custom message";
    info->addResponse<Object<VanillaSwapDto>>(
      Status::CODE_200,
      "application/json"
    );
  }
  ENDPOINT("POST", "/echo", echo, BODY_DTO(Object<VanillaSwapDto>, dto))
  {
    createSwap (dto);
    return createDtoResponse(Status::CODE_200, dto);
  }

  // TODO Insert Your endpoints here !!!
  
};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen

#endif /* SwapController_hpp */
