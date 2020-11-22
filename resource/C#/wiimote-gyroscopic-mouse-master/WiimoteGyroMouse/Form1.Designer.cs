namespace WiimoteGyroMouse
{
    partial class Form1
    {
        /// <summary>
        /// 必要なデザイナ変数です。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 使用中のリソースをすべてクリーンアップします。
        /// </summary>
        /// <param name="disposing">マネージ リソースが破棄される場合 true、破棄されない場合は false です。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows フォーム デザイナで生成されたコード

        /// <summary>
        /// デザイナ サポートに必要なメソッドです。このメソッドの内容を
        /// コード エディタで変更しないでください。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.button1 = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.btnTestMode = new System.Windows.Forms.Button();
            this.devSerialPort = new System.IO.Ports.SerialPort(this.components);
            this.serialWorker = new System.ComponentModel.BackgroundWorker();
            this.cmbCOMList = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // button1
            // 
            this.button1.Font = new System.Drawing.Font("MS UI Gothic", 20.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.button1.Location = new System.Drawing.Point(26, 24);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(192, 69);
            this.button1.TabIndex = 1;
            this.button1.Text = "CONNECT";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click_1);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("MS UI Gothic", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.label1.Location = new System.Drawing.Point(9, 121);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(23, 19);
            this.label1.TabIndex = 2;
            this.label1.Text = "　";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // btnTestMode
            // 
            this.btnTestMode.Font = new System.Drawing.Font("MS UI Gothic", 20.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
            this.btnTestMode.Location = new System.Drawing.Point(268, 24);
            this.btnTestMode.Name = "btnTestMode";
            this.btnTestMode.Size = new System.Drawing.Size(192, 69);
            this.btnTestMode.TabIndex = 3;
            this.btnTestMode.Text = "READY";
            this.btnTestMode.UseVisualStyleBackColor = true;
            this.btnTestMode.Click += new System.EventHandler(this.btnTestMode_Click);
            // 
            // devSerialPort
            // 
            this.devSerialPort.BaudRate = 115200;
            this.devSerialPort.PortName = "COM31";
            this.devSerialPort.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.devSerialPort_DataReceived);
            // 
            // serialWorker
            // 
            this.serialWorker.DoWork += new System.ComponentModel.DoWorkEventHandler(this.serialWorker_DoWork);
            // 
            // cmbCOMList
            // 
            this.cmbCOMList.FormattingEnabled = true;
            this.cmbCOMList.Location = new System.Drawing.Point(26, 154);
            this.cmbCOMList.Name = "cmbCOMList";
            this.cmbCOMList.Size = new System.Drawing.Size(192, 21);
            this.cmbCOMList.TabIndex = 4;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(26, 126);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(74, 13);
            this.label2.TabIndex = 5;
            this.label2.Text = "COM PORTS:";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(485, 198);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.cmbCOMList);
            this.Controls.Add(this.btnTestMode);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.button1);
            this.Name = "Form1";
            this.Text = "HITECH - Air Mouse Demo";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnTestMode;
        private System.IO.Ports.SerialPort devSerialPort;
        private System.ComponentModel.BackgroundWorker serialWorker;
        private System.Windows.Forms.ComboBox cmbCOMList;
        private System.Windows.Forms.Label label2;
    }
}

