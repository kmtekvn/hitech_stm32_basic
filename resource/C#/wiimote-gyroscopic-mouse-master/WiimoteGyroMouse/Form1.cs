using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices; 
using System.IO.Ports;
using Afproto;
using System.Collections.Concurrent;
using System.Threading;

namespace WiimoteGyroMouse
{
    public partial class Form1 : Form {


        [DllImport("user32.dll")]
        extern static uint SendInput(uint nInputs, INPUT[] pInputs, int cbSize);

        [StructLayout(LayoutKind.Sequential)]
        struct INPUT
        {
            public int type;
            public MOUSEINPUT mi;
        }

        [StructLayout(LayoutKind.Sequential)]
        struct MOUSEINPUT
        {
            public int dx;
            public int dy;
            public int mouseData;
            public int dwFlags;
            public int time;
            public IntPtr dwExtraInfo;
        }

        public class win32api
        {
            [DllImport("user32.dll")]
            public static extern uint keybd_event(byte bVk, byte bScan, uint dwFlags, UIntPtr dwExtraInfo);
        }

        //Boolean isADown = false;
        WiimoteState ws = new WiimoteState();
        public ObservableConcurrentQueue<byte> concurrQ;

        public Form1()
        {

            InitializeComponent();

            //Allow control calls from other threads
            Control.CheckForIllegalCrossThreadCalls = false;
            // Construct a ConcurrentQueue.
             concurrQ = new ObservableConcurrentQueue<byte>(true);
            string[] ports = SerialPort.GetPortNames();
            cmbCOMList.Items.AddRange(ports);
        }


        private void button1_Click_1(object sender, EventArgs e)
        {
            if (button1.Text.Equals("CONNECT"))
            {
                devSerialPort.PortName = cmbCOMList.Text;
                devSerialPort.Open();
                // TODO: init device communication
                button1.Text = "DISCONNECT";
               // label1.Text = "GYRO DISABLED";
                serialWorker.RunWorkerAsync();
            }
            else
            {
                serialWorker.CancelAsync();
                devSerialPort.Close();
                // TODO: disconnect communication
                button1.Text = "CONNECT";
                label1.Text = "";
            }
        }

        public void WiiControl(WiimoteState ws)
        {
            INPUT[] input = new INPUT[1];

            System.Windows.Forms.Cursor.Position = new System.Drawing.Point(ws.getGyroX(), ws.getGyroY());

            if (ws.rawWii_Z > 0)
            {
                input[0].mi.dwFlags = 0x0002;                       //Left mouse down
                SendInput(1, input, Marshal.SizeOf(input[0]));      //Send mouse event
                btnTestMode.Text = "CLICKED";

            }else
            {

                input[0].mi.dwFlags = 0x0004;                      //Left mouse up
                SendInput(1, input, Marshal.SizeOf(input[0]));     //Send mouse event
                btnTestMode.Text = "RELEASE";
            }
        }

        private void btnTestMode_Click(object sender, EventArgs e)
        {
            //isADown = false;
            ws.setGyroX(200);
            ws.setGyroY(300);
            ws.setGyroZ(400);
            WiiControl(ws);
        }

        private void processSensorFrame(byte[] framedata)
        {
            if (framedata == null) { return;  }
            string data_str = System.Text.Encoding.UTF8.GetString(framedata);

            string[] data_list = data_str.Split(',');

            if (data_list.Length >= 3)
            {
                ws.setGyroX(Int32.Parse(data_list[0]));
                ws.setGyroY(Int32.Parse(data_list[1]));
                ws.setGyroZ(Int32.Parse(data_list[2]));
                WiiControl(ws);
            }
        }

        private void devSerialPort_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            int i = 0;
            SerialPort spL = (SerialPort)sender;
            byte[] framedData = new byte[spL.BytesToRead];

            spL.Read(framedData, 0, framedData.Length);

            for (i = 0; i < framedData.Length; i++)
            {
                concurrQ.Enqueue(framedData[i]);
            }
        }

      
        private void serialWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            int i = 0;
            int number = 0;
            byte item = 0;
            const int max_queue_size = 64;

            byte[] framedData = new byte[max_queue_size];
            while (true)
            {
                if (concurrQ.TryDequeue(out item) == false)
                {
                    Thread.Sleep(300);
                    continue;
                }

                if (i >= max_queue_size) { i = 0; Array.Clear(framedData, 0, framedData.Length); }

                framedData[i++] = item;
                var deFramedData = Afproto.Deframer.getData(framedData, ref number);

                if (deFramedData != null)
                {
                    //  var output = Encoding.ASCII.GetString(deFramedData);
                    processSensorFrame(deFramedData);
                    i = 0;
                    Array.Clear(framedData, 0, framedData.Length);
                }
            }
        }
    }
}
