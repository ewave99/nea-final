#include <string.h>
#include "visuals.h"

static VisualModule
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
                .x = 300,
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
        case M_MIXER:
            strcpy(visual_module.module_name, "MIXER");

            //fill colour: #e1d5e7 = (225.0, 213.0, 231.0)
            visual_module.fill_colour[0] = 225.0 / 256;
            visual_module.fill_colour[1] = 213.0 / 256;
            visual_module.fill_colour[2] = 231.0 / 256;

            //line colour: #9673a6 = (150, 115, 166)
            visual_module.line_colour[0] = 150.0 / 256;
            visual_module.line_colour[1] = 115.0 / 256;
            visual_module.line_colour[2] = 166.0 / 256;

            visual_module.rect = (Rect){
                .x = 300,
                .y = 600,
                .w = 300,
                .h = 100
            };

            visual_module.num_inputs = 4;
            strcpy(visual_module.inputs[0], "in0");
            strcpy(visual_module.inputs[1], "in1");
            strcpy(visual_module.inputs[2], "in2");
            strcpy(visual_module.inputs[3], "in3");

            visual_module.num_outputs = 1;
            strcpy(visual_module.outputs[0], "out");
            break;
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

void
convertAbstractModulesToVisualModules(int num_modules,
        AbstractModule* abstract_modules, VisualModule* visual_modules)
{
    for (int i = 0; i < num_modules; i ++)
        visual_modules[i] = convertAbstractModuleToVisualModule(
                abstract_modules[i]);
}
