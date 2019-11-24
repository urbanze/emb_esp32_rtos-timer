#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <freertos/timers.h>
#include <esp_system.h>
#include <esp_log.h>

SemaphoreHandle_t smf;//Cria o objeto do semaforo
TimerHandle_t tmr;//Cria o objeto do timer

void ISR(void*z)
{
    int aux = 0;
    xSemaphoreGiveFromISR(smf, &aux);//Libera o semaforo para uso

    if (aux)
    {
        portYIELD_FROM_ISR();//Forca a troca de contexto se necessario
    }
}

void app_main()
{
    smf = xSemaphoreCreateBinary();//Cria o semaforo

    tmr = xTimerCreate("tmr_smf", pdMS_TO_TICKS(1000), true, 0, ISR);//Cria o timer com 1000ms de frequencia com auto-reaload
    xTimerStart(tmr, pdMS_TO_TICKS(100));//Inicia o timer

    while (1)
    {
        if (xSemaphoreTake(smf, portMAX_DELAY))
        {
            ESP_LOGI("Timer", "expirou!");//Ao obter o semaforo, enviara a string para o terminal
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
