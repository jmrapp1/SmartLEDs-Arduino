
#include <LightService.h>
#include <StateService.h>
#include <effects/Effect.h>

Effect::Effect(StateService &stateService, LightService &lightService): _stateService(stateService), _lightService(lightService) {
}
