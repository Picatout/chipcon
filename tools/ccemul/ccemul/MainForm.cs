/*
 * Created by SharpDevelop.
 * User: Jacques
 * Date: 2014-11-05
 * Time: 08:27
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
using System;
using System.IO;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;


namespace ccemul
{
	/// <summary>
	/// Description of MainForm.
	/// </summary>
	/// 
	
	
	public partial class MainForm : Form
	{

		// affichage		
		// machine virtuelle CHIPcon
		
		//message d'erreur machine virtuelle
		internal String[] error_msg= new String[7]
		{
		 "VM normal exit",
		 "VM invalid operation code",
		 "Invalid character value for selected font",
		 "Invalid tone value, range is 0-15",
		 "Program counter out of bound",
		 "Stack underflow",
		 "Stack overflow"
		};
		
		//internal ChipConVM  vm;
		internal ChipConVM vm;
		
		// test sprites
		byte[] sprite = new byte[8] {0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa};
		//trace flag
		bool trace=false;

		public MainForm()
		{
			//
			// The InitializeComponent() call is required for Windows Forms designer support.
			//
			InitializeComponent();
			//
			// TODO: Add constructor code after the InitializeComponent() call.
			//
			vm = new ChipConVM();
			vm.text = new Text(this);
			pictureBox1.Image=vm.tv.display;
			vm.tv.cls();
			traceToolStripMenuItem.Checked=trace;
		}
		
		void ToolStripMenuItem1Click(object sender, EventArgs e)
		{
			
		}
		
		void QuitToolStripMenuItemClick(object sender, EventArgs e)
		{
			Application.Exit();
		}
		
		void OpenToolStripMenuItemClick(object sender, EventArgs e)
		{
			byte[] data;
			
            openFileDialog1.Filter = "CHIPcon binary|*.bin|SCHIP files|*.SC,*.CH8|All Files (*.*)|*.*";
            openFileDialog1.FilterIndex = 1;
            openFileDialog1.Multiselect = false;
            DialogResult result=openFileDialog1.ShowDialog();
            if (result == DialogResult.OK)
            {
                System.IO.Stream fileStream = openFileDialog1.OpenFile();
                data= new byte[Math.Min(3584,fileStream.Length)];
                using (System.IO.BinaryReader reader = new System.IO.BinaryReader(fileStream))
                {
                	reader.Read(data,0,(int)data.Length);
                }
                fileStream.Close();
                vm.load(data);
            }
		}
		
		void OpenFileDialog1FileOk(object sender, System.ComponentModel.CancelEventArgs e)
		{
			
		}
		

		
		void MainFormLoad(object sender, EventArgs e)
		{
		}
		
		
		void MainFormShown(object sender, EventArgs e)
		{

			timer1.Enabled=true;
		}
		
		
		void displayVMState()
		{   
			ushort prevPC=(ushort)(vm.pc-2);
			ushort code=(ushort)((vm.code[prevPC]<<8)|vm.code[prevPC+1]);
			textBox1.AppendText(string.Format("PC={0:X}, ",prevPC));
			textBox1.AppendText(string.Format("opCode={0:X} ({1:S})\t",code,dasm.deassemble(prevPC,code)));
			textBox1.AppendText(string.Format("I={0:X}, ",vm.ix));
			textBox1.AppendText(string.Format("sp={0}, ",vm.sp));
			textBox1.AppendText(string.Format("DT={0}, ",vm.dt));
			textBox1.AppendText(string.Format("ST={0}\r\n",vm.st));
			for (int i=0;i<16;i++){
				textBox1.AppendText(string.Format("var[{0:D}]={1:D}, ",i,vm.var[i]));
			}
			textBox1.AppendText("\r\n");
			for (int i=0;i<16;i++){
				textBox1.AppendText(string.Format("rpl[{0:D}]={0:D}, ",i,vm.rpl[i]));
			}
			textBox1.Invalidate();
			
		}
		
		void Timer1Tick(object sender, EventArgs e)
		{
			vm_error error;
			
			pictureBox1.Invalidate();
			if (vm.dt>0) vm.dt--;
			if (vm.st>0) vm.st--;
			if (vm.fRunning && vm.speed==0){
				if (trace)
				{
					textBox1.Text="";
					displayVMState();
					vm.speed=1;
				}else
					vm.speed=(byte)trackBar1.Value;
				if ((error=vm.ccVM())!=vm_error.VM_OK)
				{
					
					textBox1.Text=String.Format("VM error:{0:S}\r\n",error_msg[(int)error]);
					displayVMState();
				}else if (!vm.fRunning){
					textBox1.Text="Game existed normally";
				}
			}
			//if (timer1.Interval==16) timer1.Interval=17; else timer1.Interval=16;
		}
		
		void MainFormPaint(object sender, PaintEventArgs e)
		{
			
		}
		
		void ResetToolStripMenuItemClick(object sender, EventArgs e)
		{
			vm.Reset();
			textBox1.Text="Restarted";
			vm.Resume();
		}
	
		
		void TextBox1KeyDown(object sender, KeyEventArgs e)
		{
			
		}
	
		void TrackBar1KeyDown(object sender, KeyEventArgs e)
		{
			if (vm.fRunning) vm.kpad.hexKeyDown((byte)e.KeyValue);
		}

		void TraceToolStripMenuItemClick(object sender, EventArgs e)
		{
			trace=!trace;
			traceToolStripMenuItem.Checked=trace;
		}
		
		void TrackBar1KeyUp(object sender, KeyEventArgs e)
		{
			if (vm.fRunning) vm.kpad.hexKeyUp((byte)e.KeyValue);
		}
		void AboutToolStripMenuItemClick(object sender, EventArgs e)
		{
			FormAbout about=new FormAbout();
			about.Show();
		}
		void ToolStripButton2Click(object sender, EventArgs e)
		{
			vm.fRunning=false;
			textBox1.Text="Paused\r\n";
			displayVMState();
		}
		void ToolStripButton3Click(object sender, EventArgs e)
		{
			vm.fRunning=false;
			vm.Reset();
			textBox1.Text="Stopped\r\n";
			
		}
		void ToolStripButton4Click(object sender, EventArgs e)
		{
			vm.fRunning=true;
			textBox1.Text="Resumed";
		}
	}
	

	
}
