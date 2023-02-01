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
            setModuleRect(&visual_module, 0, 0, 300, 100);
            visual_module.num_inputs = 0;
            visual_module.num_outputs = 1;
            strcpy(visual_module.outputs[0], "out");
            break;
        case M_SQUOSC:
            setModuleName(&visual_module, "SQUARE OSCILLATOR");
            setModuleFillColour256(&visual_module, 255., 242., 204.);
            setModuleLineColour256(&visual_module, 214., 182., 86.);
            setModuleRect(&visual_module, 0, 0, 300, 100);
            visual_module.num_inputs = 0;
            visual_module.num_outputs = 1;
            strcpy(visual_module.outputs[0], "out");
            break;
        case M_TRIOSC:
            setModuleName(&visual_module, "TRIANGLE OSCILLATOR");
            setModuleFillColour256(&visual_module, 255., 242., 204.);
            setModuleLineColour256(&visual_module, 214., 182., 86.);
            setModuleRect(&visual_module, 0, 0, 300, 100);
            visual_module.num_inputs = 0;
            visual_module.num_outputs = 1;
            strcpy(visual_module.outputs[0], "out");
            break;
        case M_SAWOSC:
            setModuleName(&visual_module, "SAW OSCILLATOR");
            setModuleFillColour256(&visual_module, 255., 242., 204.);
            setModuleLineColour256(&visual_module, 214., 182., 86.);
            setModuleRect(&visual_module, 0, 0, 300, 100);
            visual_module.num_inputs = 0;
            visual_module.num_outputs = 1;
            strcpy(visual_module.outputs[0], "out");
            break;
        case M_CLOCK:
            setModuleName(&visual_module, "CLOCK");
            setModuleFillColour256(&visual_module, 246.0, 148.0, 193.0);
            setModuleLineColour256(&visual_module, 120.0, 38.0, 130.0);
            setModuleRect(&visual_module, 0, 0, 200, 100);
            visual_module.num_inputs = 0;
            visual_module.num_outputs = 1;
            strcpy(visual_module.outputs[0], "out");
            break;
        case M_SEQUENCER:
            setModuleName(&visual_module, "SEQUENCER");
            setModuleFillColour256(&visual_module, 248.0, 206.0, 204.0);
            setModuleLineColour256(&visual_module, 184.0, 84.0, 80.0);
            setModuleRect(&visual_module, 0, 0, 300, 100);
            visual_module.num_inputs = 1;
            strcpy(visual_module.inputs[0], "trigger");
            visual_module.num_outputs = 1;
            strcpy(visual_module.outputs[0], "out");
            break;
        case M_ENVELOPE:
            setModuleName(&visual_module, "ENVELOPE");
            setModuleFillColour256(&visual_module, 218.0, 232.0, 252.0);
            setModuleLineColour256(&visual_module, 108.0, 142.0, 191.0);
            setModuleRect(&visual_module, 0, 0, 300, 100);
            visual_module.num_inputs = 1;
            strcpy(visual_module.inputs[0], "trigger");
            visual_module.num_outputs = 1;
            strcpy(visual_module.outputs[0], "out");
            break;
        case M_MIXER:
            setModuleName(&visual_module, "MIXER");
            setModuleFillColour256(&visual_module, 225., 213., 231.);
            setModuleLineColour256(&visual_module, 150., 115., 166.);
            setModuleRect(&visual_module, 0, 0, 300, 100);
            visual_module.num_inputs = 4;
            strcpy(visual_module.inputs[0], "in0");
            strcpy(visual_module.inputs[1], "in1");
            strcpy(visual_module.inputs[2], "in2");
            strcpy(visual_module.inputs[3], "in3");
            visual_module.num_outputs = 1;
            strcpy(visual_module.outputs[0], "out");
            break;
        case M_LPF:
            setModuleName(&visual_module, "LPF");
            setModuleFillColour256(&visual_module, 255., 255., 255.);
            setModuleLineColour256(&visual_module, 0., 0., 0.);
            setModuleRect(&visual_module, 0, 0, 300, 100);
            visual_module.num_inputs = 1;
            strcpy(visual_module.inputs[0], "in");
            visual_module.num_outputs = 1;
            strcpy(visual_module.outputs[0], "out");
            break;
        case M_HPF:
            setModuleName(&visual_module, "HPF");
            setModuleFillColour256(&visual_module, 255., 255., 255.);
            setModuleLineColour256(&visual_module, 0., 0., 0.);
            setModuleRect(&visual_module, 0, 0, 300, 100);
            visual_module.num_inputs = 1;
            strcpy(visual_module.inputs[0], "in");
            visual_module.num_outputs = 1;
            strcpy(visual_module.outputs[0], "out");
            break;
        case M_REVERB:
            setModuleName(&visual_module, "REVERB");
            setModuleFillColour256(&visual_module, 255., 255., 255.);
            setModuleLineColour256(&visual_module, 0., 0., 0.);
            setModuleRect(&visual_module, 0, 0, 300, 100);
            visual_module.num_inputs = 1;
            strcpy(visual_module.inputs[0], "in");
            visual_module.num_outputs = 1;
            strcpy(visual_module.outputs[0], "out");
            break;
        case M_AUDIOFILE:
            setModuleName(&visual_module, "AUDIOFILE");
            setModuleFillColour256(&visual_module, 255., 255., 255.);
            setModuleLineColour256(&visual_module, 0., 0., 0.);
            setModuleRect(&visual_module, 0, 0, 300, 100);
            visual_module.num_inputs = 0;
            visual_module.num_outputs = 1;
            strcpy(visual_module.outputs[0], "out");
            break;
        case M_SPEAKERS:
            setModuleName(&visual_module, "SPEAKERS");
            setModuleFillColour256(&visual_module, 255., 255., 255.);
            setModuleLineColour256(&visual_module, 0., 0., 0.);
            setModuleRect(&visual_module, 0, 0, 300, 100);
            visual_module.num_inputs = 1;
            strcpy(visual_module.inputs[0], "in");
            visual_module.num_outputs = 0;
            break;
        default:
            setModuleName(&visual_module, "UNDEFINED");
            setModuleFillColour256(&visual_module, 255., 255., 255.);
            setModuleLineColour256(&visual_module, 0., 0., 0.);
            setModuleRect(&visual_module, 0, 0, 300, 100);
            visual_module.num_inputs = 1;
            strcpy(visual_module.inputs[0], "in");
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

void
arrangeModulesOnCanvas(int canvas_width, int canvas_height, int num_modules,
        VisualModule *modules, int num_connections,
        Connection *connections)
{
    int num_vertical_sections = num_modules;
    int vertical_section_height = canvas_height / num_vertical_sections;

    for (int module_index = 0; module_index < num_modules; module_index ++)
    {
        modules[module_index].rect.x = (canvas_width
                - modules[module_index].rect.w) / 2;
        modules[module_index].rect.y = vertical_section_height * module_index;
    }
}
