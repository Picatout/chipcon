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
			textBox1.Text=textBox1.Text.Insert(textBox1.TextLength,string.Format("PC={0,3:X} ",vm.pc));
			textBox1.Text=textBox1.Text.Insert(textBox1.TextLength,string.Format("I={0,3:X} ",vm.ix));
			textBox1.Text=textBox1.Text.Insert(textBox1.TextLength,string.Format("sp={0} ",vm.sp));
			textBox1.Text=textBox1.Text.Insert(textBox1.TextLength,string.Format("DT={0} ",vm.dt));
			textBox1.Text=textBox1.Text.Insert(textBox1.TextLength,string.Format("opCode={0,4:X} ",(vm.code[vm.pc]<<8)+vm.code[vm.pc+1]));
			textBox1.Invalidate();
			
		}
		
		void Timer1Tick(object sender, EventArgs e)
		{
			pictureBox1.Invalidate();
			if (vm.dt>0) vm.dt--;
			if (vm.st>0) vm.st--;
			if (vm.Running && vm.speed==0){
				if (trace)
				{
					textBox1.Text="";
					displayVMState();
					vm.speed=1;
				}else
					vm.speed=(byte)trackBar1.Value;
			}
			if (vm.ccVM()==ChipConVM.CHIP_BAD_OPCODE)
			{
				
				textBox1.Text="opcode error:\r\n";
				displayVMState();
			}
			//if (timer1.Interval==16) timer1.Interval=17; else timer1.Interval=16;
		}
		
		void MainFormPaint(object sender, PaintEventArgs e)
		{
			
		}
		
		void ResetToolStripMenuItemClick(object sender, EventArgs e)
		{
			vm.Reset();
		}
	
		
		void TextBox1KeyDown(object sender, KeyEventArgs e)
		{
			
		}
	
		void TrackBar1KeyDown(object sender, KeyEventArgs e)
		{
			if (vm.Running && vm.kpad.hexKey((byte)e.KeyValue)) vm.tone.play_tone(1000,2,false);
		}

		void TraceToolStripMenuItemClick(object sender, EventArgs e)
		{
			trace=!trace;
			traceToolStripMenuItem.Checked=trace;
		}
	}
	

	
}
