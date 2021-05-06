//-----------------------------------------------------------------------------
// Duckling.cpp - Eende-kuiken (volgt de moeder)
//-----------------------------------------------------------------------------
#include "RobotSettings.h"
#include "Libs/MyRobot.h"

//---------------------------------------------------------------------------------------
// LppSensorDucklingSetup -
//---------------------------------------------------------------------------------------
// 120..240 degrees view (-60 to 60, forward looking)
// Divided into 8 segments to keep track of mother
// when there are other, closer objects in view.
//---------------------------------------------------------------------------------------
void LppSensorDucklingSetup()
{
   Lpp.SensorSetup(0, 120, 15);  // 120->135
   Lpp.SensorSetup(1, 135, 15);
   Lpp.SensorSetup(2, 150, 15);
   Lpp.SensorSetup(3, 165, 15);
   Lpp.SensorSetup(4, 180, 15);
   Lpp.SensorSetup(5, 195, 15);
   Lpp.SensorSetup(6, 210, 15);
   Lpp.SensorSetup(7, 225, 15);  // 225->240
}

//-----------------------------------------------------------------------------
// MissieDuckling -
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool MissieDuckling(TState &S)
{
   S.Update("Duckling");

   switch (S.State) {
      case 0 : {  // LIDAR-STARTEN
         if (S.NewState) {
            LppSensorDucklingSetup();     // reconfigure Lpp
            Lpp.Start();
         }

         if (S.StateTime() > 2000) {      // Wacht op start lidar
            S.State++;
         }
      }
      break;


      case 1 : {  // Follow mother
         if (S.NewState) {
            // find mother
            int Distance = 9999;
            int Degrees32 = 0;
            for (int i=0; i< 7; i++) {
               if (Distance > Lpp.Sensor[i].Distance) {
                  Distance   = Lpp.Sensor[i].Distance;
                  Degrees32  = Lpp.Sensor[i].Degrees32;
               }
            }
            CSerial.printf("Mother at %d mm, %d degrees, sensor: %d \n", Distance, Degrees32, (Degrees32 - 120) / 15 / 32);
         }

         if (S.StateTime() > 2000) {      // Wacht op start lidar
            S.State++;
         }
      }
      break;


      default : {
         CSerial.printf("Error: ongeldige state in MissieTemplate (%d)\n", S.State);
         return true;  // error => mission end
      }
   }
   return false;  // mission nog niet gereed
}

