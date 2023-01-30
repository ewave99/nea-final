#include <string.h>

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

typedef struct {
    int x;
    int y;
    int w;
    int h;
} Rect;

typedef struct
{
    char module_name[16];
    Rect rect;
    double fill_colour[3];
    double line_colour[3];
    int num_inputs;
    char inputs[8][16];
    int num_outputs;
    char outputs[8][16];
}
VisualModule;

VisualModule convertAbstractModuleToVisualModule(
        AbstractModule abstract_module);
