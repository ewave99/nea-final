#include <string.h>
#include "drawing.h"

typedef enum {
    M_SINOSC,
    M_SQUOSC,
    M_TRIOSC,
    M_SAWOSC,
    M_CLOCK,
    M_SEQUENCER,
    M_ENVELOPE,
    M_MIXER,
    M_LPF,
    M_HPF,
    M_REVERB,
    M_AUDIOFILE
} ModuleType;

typedef struct
{
    ModuleType module_type;
} AbstractModule;

VisualModule
convertAbstractModuleToVisualModule(AbstractModule abstract_module)
{
    VisualModule visual_module;
    
    switch (abstract_module.module_type)
    {
        case M_SINOSC:
            strcpy(visual_module.module_name, "SINE OSCILLATOR");

            //fill colour: #fff2cc = (255, 242, 204)
            visual_module.fill_colour[0] = 255.0 / 256;
            visual_module.fill_colour[1] = 242.0 / 256;
            visual_module.fill_colour[2] = 204.0 / 256;

            //line colour: #d6b656 = (214, 182, 86)
            visual_module.line_colour[0] = 214.0 / 256;
            visual_module.line_colour[1] = 182.0 / 256;
            visual_module.line_colour[2] = 86.0 / 256;

            visual_module.rect = (Rect){
                .x = 0,
                .y = 0,
                .w = 300,
                .h = 100
            };

            visual_module.num_inputs = 0;

            visual_module.num_outputs = 1;
            strcpy(visual_module.outputs[0], "out");
            break;
            
        //case M_SQUOSC:
        //case M_TRIOSC:
        //case M_SAWOSC:
        //case M_CLOCK:
        //case M_SEQUENCER:
        //case M_ENVELOPE:
        //case M_MIXER:
        //case M_LPF:
        //case M_HPF:
        //case M_REVERB:
        //case M_AUDIOFILE:
        default:
            strcpy(visual_module.module_name, "UNDEFINED");

            visual_module.fill_colour[0] = 1;
            visual_module.fill_colour[1] = 1;
            visual_module.fill_colour[2] = 1;

            visual_module.line_colour[0] = 0;
            visual_module.line_colour[1] = 0;
            visual_module.line_colour[2] = 0;

            visual_module.rect = (Rect){ .x = 0, .y = 0, .w = 100, .h = 100 };

            visual_module.num_inputs = 0;

            visual_module.num_outputs = 1;
            strcpy(visual_module.outputs[0], "out");

            break;
    }

    return visual_module;
}
