
#include <BlynkGOv5.h>

class GMyGaugeRainbow : public GGaugeRainbow 
{
  public:
    GMyGaugeRainbow(GWidget&parent=GScreen) : GGaugeRainbow(parent) {}
    String _title;
    String _unit;

    void create(){
      if(this->isCreated()) return;
      GGaugeRainbow::create();
      GGaugeRainbow::hookDrawTask(true);
      GGaugeRainbow::onDrawTask(GWIDGET_CB{
        
      });
    } 
};

void setup()
{
  Serial.begin(9600); Serial.println();
  BlynkGO.begin();

  gauge = 89.43;

}

void loop()
{
  BlynkGO.update();
}