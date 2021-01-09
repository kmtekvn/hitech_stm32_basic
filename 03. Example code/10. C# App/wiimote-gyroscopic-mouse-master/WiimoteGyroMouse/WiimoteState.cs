using System;

namespace WiimoteGyroMouse
{
    public class WiimoteState
    {
        const int GYRO_NEUTRAL_X = 7859;
        const int GYRO_NEUTRAL_Y = 7893;
        const int GYRO_NEUTRAL_Z = 7825;

        public int rawWii_X = 0;
        public int rawWii_Y = 0;
        public int rawWii_Z = 0;

        public object ButtonState { get; internal set; }
        public void setGyroX(int gyro_val)
        {
           rawWii_X = gyro_val;
        }

        public void setGyroY(int gyro_val)
        {
            rawWii_Y = gyro_val;
        }

        public void setGyroZ(int gyro_val)
        {
            rawWii_Z = gyro_val;
        }

        public int getGyroX() {

            /*       int rawValue = 7859 - rawWii_X;
                   int sign = rawValue < 0 ? -1 : 1;

                   return sign * (int)(120.0 * Math.Pow((double)Math.Abs(rawValue) / 7859.0, 1.3));
            */
            return rawWii_X;
         }

        public int getGyroY()
          {
            /*      int rawValue = 7893 - rawWii_Y;
                  int sign = rawValue < 0 ? 1 : -1;

                  return sign * (int)(120.0 * Math.Pow((double)Math.Abs(rawValue) / 7893.0, 1.3));
            */
            return rawWii_Y;
        }

        public int getGyroZ()
        {
         /*   int rawValue = 7825 - rawWii_Z;
            int sign = rawValue < 0 ? 1 : -1;
            return sign * (int)(120.0 * Math.Pow((double)Math.Abs(rawValue) / 7825.0, 1.3));
         */
            return rawWii_Z;
        }
    }
}