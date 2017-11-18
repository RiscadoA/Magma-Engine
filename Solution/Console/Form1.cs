using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.IO.Pipes;

namespace Console {
    public partial class Form1 : Form {
        private NamedPipeClientStream client;
        private NamedPipeServerStream server;
        private StreamWriter writer;
        private Color defaultTextColor;
        private Color errorTextColor = Color.Red;
        private Color warningTextColor = Color.DarkOrange;

        private const int CP_NOCLOSE_BUTTON = 0x200;
        protected override CreateParams CreateParams {
            get {
                CreateParams cp = base.CreateParams;
                cp.ClassStyle = cp.ClassStyle | CP_NOCLOSE_BUTTON;
                return cp;
            }
        }

        public Form1() {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e) {
            server = new NamedPipeServerStream("Magma_Console_Pipe_Send", PipeDirection.Out);
            client = new NamedPipeClientStream(".", "Magma_Console_Pipe_Read", PipeDirection.In);

            defaultTextColor = outputText.ForeColor;

            try {
                client.Connect(2000);
            }
            catch (TimeoutException) {
                MessageBox.Show("Failed to init Console, failed to connect to Magma");
                this.Close();
                return;
            }

            server.WaitForConnection();
            writer = new StreamWriter(server);

            inputText.Select();

            Read();
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e) {
            if (server.IsConnected)
                Send("/exit");
        }

        // Execute message
        private void button1_Click(object sender, EventArgs e) {
            Execute(inputText.Text);
            inputText.Clear();
        }

        // Clear console
        private void button2_Click(object sender, EventArgs e) {
            Clear();
        }

        public void Print(string text) {
            outputText.AppendText(text.Replace("\n", Environment.NewLine), defaultTextColor);
        }

        public void Error(string text) {
            outputText.AppendText(text.Replace("\n", Environment.NewLine), errorTextColor);
        }

        public void Warning(string text) {
            outputText.AppendText(text.Replace("\n", Environment.NewLine), warningTextColor);
        }

        public void Execute(string text) {
            string str = inputText.Text;
            inputText.Text = "";
            if (String.IsNullOrWhiteSpace(str))
                return;
            Print("> " + str + "\n");
            Send(str);
        }

        public void Send(string text) {
            writer.Write(text);
            writer.Flush();
        }

        public void Clear() {
            outputText.Clear();
        }

        public void Read() {
            if (!client.IsConnected) {
                this.Close();
                return;
            }

            byte[] buffer = new byte[1024 * 16];
            try {
                client.BeginRead(buffer, 0, buffer.Length, ReadCallback, buffer);
            }
            catch (Exception) {
                this.Close();
                return;
            }
        }

        public void ReadCallback(IAsyncResult ar) {
            int bytesRead = 0;

            try {
                bytesRead = client.EndRead(ar);
            }
            catch (Exception e) {
                MessageBox.Show("Exception caught: " + e.Message);
                this.Close();
                return;
            }

            if (bytesRead > 0) {
                byte[] data = (byte[])ar.AsyncState;

                StreamReader stream = new StreamReader(new MemoryStream(data, 0, bytesRead), Encoding.UTF8);

                while (!stream.EndOfStream) {
                    string str = "";
                    while (stream.Peek() != 0)
                        str += (char)stream.Read();
                    stream.Read();
                    if (str != "") {
                        if (str == "close") {
                            this.Close();
                            return;
                        }
                        else if (str == "clear")
                            this.Clear();
                        else if (str.Length >= 5 && str.Substring(0, 5) == "print")
                            Print(str.Substring(6));
                        else if (str.Length >= 5 && str.Substring(0, 5) == "error")
                            Error(str.Substring(6));
                        else if (str.Length >= 5 && str.Substring(0, 5) == "warng")
                            Warning(str.Substring(6));
                        else Error(Environment.NewLine + "UNKNOWN MSG RECEIVED \"" + str + "\"" + Environment.NewLine);
                    }
                }
            }

            this.Read();
        }
    }
}

public static class RichTextBoxExtensions {
    public static void AppendText(this RichTextBox box, string text, Color color) {
        box.SelectionStart = box.TextLength;
        box.SelectionLength = 0;

        box.SelectionColor = color;
        box.AppendText(text);
        box.SelectionColor = box.ForeColor;
    }
}