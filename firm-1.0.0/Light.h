#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include "jfunc.h"

class Light
{
    public:

    int dow;
    int cycle;
    int epoch;

    int dayLedPins[7]={7,1,0,2,11,9,8};
    int moonLedPins[8]={26,27,28,29,25,24,18,23};

    int bLED=3;
    int rLED=4;
    int gLED=5;

    int rVal=0;
    int gVal=0;
    int bVal=0;

    uint32_t pwm_set_freq_duty(uint slice_num, uint chan, uint32_t f, int d)
    {
        uint32_t clock = 12000000;
        uint32_t divider16 = clock / f / 4096 + (clock % (f * 4096) != 0);
        if (divider16 / 16 == 0)
            divider16 = 16;
        uint32_t wrap = clock * 16 / divider16 / f - 1;
        pwm_set_clkdiv_int_frac(slice_num, divider16 / 16, divider16 & 0xF);
        pwm_set_wrap(slice_num, wrap);
        pwm_set_chan_level(slice_num, chan, wrap * d / 100);
        return wrap;
    }

    void setLights(int dow, int cycle, int epoch)
    {
				//------------------------------------
        for(int i=0; i<7; i++)
        {
            gpio_init(dayLedPins[i]);
            gpio_set_dir(dayLedPins[i],GPIO_OUT);
        }

        for(int i=0; i<8; i++)
        {
            gpio_init(moonLedPins[i]);
            gpio_set_dir(moonLedPins[i],GPIO_OUT);
        }

        gpio_put(dayLedPins[dow],1);
        gpio_put(moonLedPins[cycle],1);

        switch(epoch)
        {
            case 0:
                rVal=250;
                gVal=70;   
                bVal=70;
                printf("case 0");
                break;

            case 1:
                rVal=67;
                gVal=160;   
                bVal=255;
                printf("case 1");
                break;

            case 6:
                rVal=255;
                gVal=255;   
                bVal=30;
                printf("yellow");
                break;

            case 7:
                rVal=138;
                gVal=40;   
                bVal=150;
                printf("purple");
                break;
            case 8:
                rVal=67;
                gVal=190;   
                bVal=95;
                printf("case 1");
                break;
        }

        rVal=(rVal*100)/255;
        gVal=(gVal*100)/255;
        bVal=(bVal*100)/255;

		//------------------------------------
				gpio_set_function(rLED, GPIO_FUNC_PWM);
				gpio_set_function(gLED, GPIO_FUNC_PWM);
				gpio_set_function(bLED, GPIO_FUNC_PWM);

        uint slice_r = pwm_gpio_to_slice_num(rLED);
        uint chan_r = pwm_gpio_to_channel(rLED);
        uint slice_g = pwm_gpio_to_slice_num(gLED);
        uint chan_g = pwm_gpio_to_channel(gLED);
        uint slice_b = pwm_gpio_to_slice_num(bLED);
        uint chan_b = pwm_gpio_to_channel(bLED);

				//uint32_t freq = 50;
                
                uint32_t freq = 255;
                int duty_cycle = 75;


                float gamma=2.2; 

				//rVal+=4;
				//gVal=;
				//bVal+=18;


				rVal-=14;
				gVal-=18;

                if(rVal<0)
                {
                    rVal=0;
                }

                if(gVal<0)
                {
                    gVal=0;
                }

                printf("rval %d",rVal);

                float dim=1;

                int rValCorrected=pow(rVal/100.0,gamma)*100.0*dim;
                int gValCorrected=pow(gVal/100.0,gamma)*100.0*dim;
                int bValCorrected=pow(bVal/100.0,gamma)*100.0*dim;

                printf("rvalcor %d",rValCorrected);

				//pwm_set_freq_duty(slice_r, chan_r, freq, duty_cycle);
        pwm_set_freq_duty(slice_r, chan_r, freq, rValCorrected);
        pwm_set_freq_duty(slice_g, chan_g, freq, gValCorrected);
        pwm_set_freq_duty(slice_b, chan_b, freq, bValCorrected);

        pwm_set_enabled(slice_r, true);
        pwm_set_enabled(slice_g, true);
        pwm_set_enabled(slice_b, true);

        printf("vals: R: %d, G: %d, B: %d\n", rVal, gVal, bVal);
    }
};
