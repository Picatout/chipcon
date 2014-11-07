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
	
	internal enum eOP {eWHITE,eBLACK,eINVERT};

	

	public partial class MainForm : Form
	{

		// affichage		
		// machine virtuelle CHIPcon
		ChipConVM  vm;
		
		// test sprites
		byte[] sprite = new byte[8] {0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55};
		
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
			vm.tv=new TVout();
			pictureBox1.Image=vm.tv.display;
			vm.tv.cls();
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
			Stream file;
			file=openFileDialog1.OpenFile();
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
			vm.speed=(byte)trackBar1.Value;
			vm.tv.putSprite(64,32,8,8, sprite);
			vm.Run();
			
		}
		
		

		
		void Timer1Tick(object sender, EventArgs e)
		{
			pictureBox1.Invalidate();
			if (vm.dt>0) vm.dt--;
			if (vm.st>0) vm.st--;
			vm.tv.FrameCounter++;
			if (vm.speed==0)
			{
				vm.speed=(byte)trackBar1.Value;
				vm.Run();
			}
			if (timer1.Interval==16) timer1.Interval=17; else timer1.Interval=16;
			//timer1.Enabled=true;
		}
		
		void MainFormPaint(object sender, PaintEventArgs e)
		{
			
		}
		
		void ResetToolStripMenuItemClick(object sender, EventArgs e)
		{
			vm.Reset();
		}
	}
	
}
