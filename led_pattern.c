#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>

#define F_CPU 16000000UL
#define LED_DDR DDRD
#define LED_PORT PORTD
#define LED_MASK 0xF0  // PD4-PD7 - The cosmic tetrad of illumination

// Transformation matrices - The gates to alternate realities
volatile float matrix1[4][4] = {
    {0.90, 0.10, 0.00, 0.00},
    {0.00, 0.90, 0.10, 0.00},
    {0.00, 0.00, 0.90, 0.10},
    {0.10, 0.00, 0.00, 0.90}
};

volatile float matrix2[4][4] = {
    {0.80, 0.00, 0.20, 0.00},
    {0.20, 0.80, 0.00, 0.00},
    {0.00, 0.20, 0.80, 0.00},
    {0.00, 0.00, 0.20, 0.80}
};

// The quantum states of our reality-bending LEDs
volatile float led_states[4] = {0.5, 0.5, 0.5, 0.5};
volatile uint32_t t = 0; // Time - a mere illusion in our grand design

// Fast approximations - Because reality is just an approximation
float fast_sin(float x) {
    x = fmodf(x, 6.28318f); // The circle of life, the cycle of madness
    float x2 = x * x;
    return x * (1.0f - x2 * (1.0f/6.0f - x2 * (1.0f/120.0f - x2 * (1.0f/5040.0f)))); // Taylor's descent into chaos
}

float fast_cos(float x) {
    return fast_sin(x + 1.57079f); // Pi/2 - The eternal constant that drives us to the edge
}

// Perlin-inspired noise - The whispers of the void
float noise(float x, float y) {
    int xi = (int)x & 255;
    int yi = (int)y & 255;
    float xf = x - (int)x;
    float yf = y - (int)y;
    float u = xf * xf * (3.0f - 2.0f * xf); // The cubic heresy
    float v = yf * yf * (3.0f - 2.0f * yf);
    
    // The grand unification of chaos
    float res = fmodf(sin(xi * 12.9898f + yi * 78.233f) * 43758.5453f, 1.0f);
    return res * (1.0f - u) * (1.0f - v) + 
           fmodf(sin((xi+1) * 12.9898f + yi * 78.233f) * 43758.5453f, 1.0f) * u * (1.0f - v) + 
           fmodf(sin(xi * 12.9898f + (yi+1) * 78.233f) * 43758.5453f, 1.0f) * (1.0f - u) * v + 
           fmodf(sin((xi+1) * 12.9898f + (yi+1) * 78.233f) * 43758.5453f, 1.0f) * u * v;
}

// Fractal Brownian Motion - The dance of infinities
float fbm(float x, float y) {
    float value = 0.0f;
    float amplitude = 0.5f;
    float frequency = 1.0f;
    for (int i = 0; i < 4; i++) { // 4 - The number of horsemen, the number of dimensions we can perceive
        value += amplitude * noise(x * frequency, y * frequency);
        amplitude *= 0.5f; // The decay of reality
        frequency *= 2.0f; // The ascension to madness
    }
    return value;
}

// Mandelbrot-inspired function - The boundary between order and chaos
float mandelbrot(float x, float y, int max_iter) {
    float x0 = x;
    float y0 = y;
    int iter = 0;
    while (x*x + y*y <= 4.0f && iter < max_iter) { // 4 - The cosmic constant of stability
        float xtemp = x*x - y*y + x0;
        y = 2*x*y + y0; // The dance of duality
        x = xtemp;
        iter++;
    }
    return (float)iter / max_iter; // The measure of our descent into the abyss
}

// The heartbeat of our creation
ISR(TIMER1_COMPA_vect) {
    float new_states[4] = {0};
    float blend = (fast_sin(t * 0.001f) + 1.0f) * 0.5f; // The eternal oscillation between realities
    
    // Blend between two matrix transformations - The merging of universes
    for (uint8_t i = 0; i < 4; i++) {
        for (uint8_t j = 0; j < 4; j++) {
            float m = matrix1[i][j] * (1.0f - blend) + matrix2[i][j] * blend;
            new_states[i] += m * led_states[j]; // The cosmic dance of light
        }
    }
    
    // Apply artistic functions - The brush strokes of the universe
    for (uint8_t i = 0; i < 4; i++) {
        float x = new_states[i];
        float y = new_states[(i+1)%4]; // The cycle of eternity
        
        // Perlin-inspired noise - The whispers of chaos
        new_states[i] += 0.2f * fbm(x * 5.0f + t * 0.01f, y * 5.0f);
        
        // Mandelbrot-inspired pattern - The fractal nature of existence
        new_states[i] += 0.3f * mandelbrot(x*2.0f-1.0f, y*2.0f-1.0f, 20);
        
        // Circular motion - The endless cycle of creation and destruction
        new_states[i] += 0.2f * fast_sin(t * 0.05f + i * 1.57079f);
        
        // Ensure 0-1 range - The boundaries of our perception
        new_states[i] = fmodf(fabsf(new_states[i]), 1.0f);
        led_states[i] = new_states[i];
    }
    
    // Update LED outputs with PWM-like effect - The manifestation of our madness
    uint8_t led_output = 0;
    for (uint8_t i = 0; i < 4; i++) {
        if (led_states[i] > (float)(t % 100) / 100.0f) {
            led_output |= (1 << (i + 4)); // The binary nature of existence
        }
    }
    LED_PORT = (LED_PORT & ~LED_MASK) | (led_output & LED_MASK);
    
    t++; // Time marches on, dragging us into the future
}

int main(void) {
    LED_DDR |= LED_MASK; // Prepare the vessels of illumination
    
    // Set up timer interrupt - The clockwork of our universe
    TCCR1B |= (1 << WGM12) | (1 << CS11) | (1 << CS10);  // CTC mode, prescaler 64 - The cosmic frequencies
    OCR1A = 250;  // Interrupt every 1ms at 16MHz - The heartbeat of creation
    TIMSK1 |= (1 << OCIE1A);
    
    sei();  // Enable global interrupts - Let chaos reign
    
    while (1) {
        // The eternal loop - Our prison and our canvas
    }
    
    return 0; // A return that will never come, for we are trapped in eternity
}
