#include <string.h>
#include "visuals.h"

static void
setModuleName(VisualModule *module, const char *name)
{
    strcpy(module->module_name, name);
}

static void
setModuleFillColour256(VisualModule *module, double r, double g, double b)
{
    module->fill_colour[0] = r / 256;
    module->fill_colour[1] = g / 256;
    module->fill_colour[2] = b / 256;
}

static void
setModuleLineColour256(VisualModule *module, double r, double g, double b)
{
    module->line_colour[0] = r / 256;
    module->line_colour[1] = g / 256;
    module->line_colour[2] = b / 256;
}

static void
setModuleRect(VisualModule *module, int x, int y, int w, int h)
{
    module->rect.x = x;
    module->rect.y = y;
    module->rect.w = w;
    module->rect.h = h;
}

static VisualModule
convertAbstractModuleToVisualModule(AbstractModule abstract_module)
{
    VisualModule visual_module;
    
    switch (abstract_module.module_type)
    {
        case M_SINOSC:
            setModuleName(&visual_module, "SINE OSCILLATOR");
            setModuleFillColour256(&visual_module, 255., 242., 204.);
            setModuleLineColour256(&visual_module, 214., 182., 86.);
            setModuleRect(&visual_module, 300, 0, 300, 100);
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
            setModuleName(&visual_module, "MIXER");
            setModuleFillColour256(&visual_module, 225., 213., 231.);
            setModuleLineColour256(&visual_module, 150., 115., 166.);
            setModuleRect(&visual_module, 300, 600, 300, 100);

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
