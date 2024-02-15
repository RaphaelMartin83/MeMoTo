#ifndef STACKINGDEFINITIONS_H
#define STACKINGDEFINITIONS_H

static const float CONNECTIONS_Z_DEPTH = 1000.0F; // Connectors shall always be visible

// For diagrams with manual stacking
static const float DEFAULT_Z_DEPTH = 500.0F; // Foreground
static const float MIN_Z_DEPTH = 50.0F; // Background
static const unsigned int MAX_PLAN = static_cast<unsigned short>(DEFAULT_Z_DEPTH - MIN_Z_DEPTH);

static float getDepthFromPlan(unsigned short p_Plan)
{
    return DEFAULT_Z_DEPTH - static_cast<float>(p_Plan + 1U);
}

#endif // STACKINGDEFINITIONS_H
