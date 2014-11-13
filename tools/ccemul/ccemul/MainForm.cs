/*---------------------------------------------------------------------------
* Copyright 2014, Jacques Deschênes
* This file is part of CHIPcon.
*
*     CHIPcon is free software: you can redistribute it and/or modify
*     it under the terms of the GNU General Public License as published by
*     the Free Software Foundation, either version 3 of the License, or
*     (at your option) any later version.
*
*     CHIPcon is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
*     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*     GNU General Public License for more details.
*
*     You should have received a copy of the GNU General Public License
*     along with CHIPcon.  If not, see <http://www.gnu.org/licenses/>.
*
*--------------------------------------------------------------------------
*/
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

 	    
		FormBreakPoints BreaksForm;
		
		//internal ChipConVM  vm;
		internal ChipConVM vm;
		
		//name of game loaded
		string gameName=null;
		
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
			conState=eCSTATE.IDLE;
			SetMenuState();
			BreaksForm = new FormBreakPoints(this);
		}
		
		//états de la console
		enum eCSTATE {IDLE,RUNNING,PAUSED,STOPPED,STEP};
		//état actuel de la console
		eCSTATE conState;
		//active/désactive les menus en fonction de conState
		void SetMenuState(){
			switch(conState){
				case eCSTATE.IDLE:
					RestartMenuItem.Enabled=false;
					BreakPointMenuItem.Enabled=false;
					clearBreakPointsMenuItem.Enabled=false;
					PauseToolBtn.Enabled=false;
					StopToolBtn.Enabled=false;
					ResumeToolBtn.Enabled=false;
					RestartToolBtn.Enabled=false;
					StepToolBtn.Enabled=false;
	                timer1.Enabled=false;
					break;
				case eCSTATE.RUNNING:
					RestartMenuItem.Enabled=true;
					BreakPointMenuItem.Enabled=true;
					clearBreakPointsMenuItem.Enabled=true;
					PauseToolBtn.Enabled=true;
					StopToolBtn.Enabled=true;
					ResumeToolBtn.Enabled=true;
					RestartToolBtn.Enabled=true;
					StepToolBtn.Enabled=false;
	                timer1.Enabled=true;
					break;
				case eCSTATE.PAUSED:
					RestartMenuItem.Enabled=true;
					BreakPointMenuItem.Enabled=true;
					clearBreakPointsMenuItem.Enabled=true;
					PauseToolBtn.Enabled=false;
					StopToolBtn.Enabled=true;
					ResumeToolBtn.Enabled=true;
					RestartToolBtn.Enabled=true;
					StepToolBtn.Enabled=true;
	                timer1.Enabled=false;
					break;
				case eCSTATE.STOPPED:
					RestartMenuItem.Enabled=true;
					BreakPointMenuItem.Enabled=true;
					clearBreakPointsMenuItem.Enabled=true;
					PauseToolBtn.Enabled=false;
					StopToolBtn.Enabled=false;
					ResumeToolBtn.Enabled=false;
					RestartToolBtn.Enabled=true;
					StepToolBtn.Enabled=true;
	                timer1.Enabled=false;
					break;
				case eCSTATE.STEP:
					RestartMenuItem.Enabled=true;
					BreakPointMenuItem.Enabled=true;
					clearBreakPointsMenuItem.Enabled=true;
					PauseToolBtn.Enabled=false;
					StopToolBtn.Enabled=false;
					ResumeToolBtn.Enabled=false;
					RestartToolBtn.Enabled=false;
					StepToolBtn.Enabled=false;
	                timer1.Enabled=true;
					break;
			}
		
		}
		
		
		void ToolStripMenuItem1Click(object sender, EventArgs e)
		{
			
		}
	
		void QuitMenuItemClick(object sender, EventArgs e)
		{
			Application.Exit();
		}
		
		void OpenFileMenuItemClick(object sender, EventArgs e)
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
                textBox1.Text="Running";
                conState=eCSTATE.RUNNING;
                SetMenuState();
                BreaksForm.ClearList();
                reloadLastFileMenuItem.Enabled=true;
            }
            gameName=openFileDialog1.FileName.Split(new char[]{'.'})[0];
            string lbl_file=gameName+".lbl";
            BreaksForm.LoadLabels(lbl_file);
            string[] pathSplit= gameName.Split(new char[]{'\\'});
            this.Text=String.Format("CHIPcon emulator ( {0:S} )", pathSplit[pathSplit.Length-1]);
		}
		
		void OpenFileDialog1FileOk(object sender, System.ComponentModel.CancelEventArgs e)
		{
			
		}
		
		
		void MainFormShown(object sender, EventArgs e)
		{
			textBox1.Text="console idle";
			conState=eCSTATE.IDLE;
			SetMenuState();
		}
		
		
		void displayVMState(ushort pc)
		{   
			ushort code=(ushort)((vm.code[pc]<<8)|vm.code[pc+1]);
			textBox1.AppendText(string.Format("PC={0:X3}, ",pc));
			textBox1.AppendText(string.Format("opCode={0:X4} ({1:S})\t",code,dasm.deassemble(pc,code)));
			textBox1.AppendText(string.Format("I={0:X3}, ",vm.ix));
			textBox1.AppendText(string.Format("sp={0}, ",vm.sp));
			textBox1.AppendText(string.Format("DT={0}, ",vm.dt));
			textBox1.AppendText(string.Format("ST={0}\r\n",vm.st));
			for (int i=0;i<16;i++){
				textBox1.AppendText(string.Format("var[{0:D}]={1:D}, ",i,vm.var[i]));
				if (i==9) textBox1.AppendText("\r\n");
			}
			textBox1.AppendText("\r\n");
			for (int i=0;i<16;i++){
				textBox1.AppendText(string.Format("rpl[{0:D}]={0:D}, ",i,vm.rpl[i]));
				if (i==9) textBox1.AppendText("\r\n");
			}
			textBox1.Invalidate();
			
		}
		
		void Timer1Tick(object sender, EventArgs e)
		{
			vm_error error;
			
			pictureBox1.Invalidate();
			if (vm.dt>0) vm.dt--;
			if (vm.st>0) vm.st--;
			switch (conState)
			{
				case eCSTATE.RUNNING:
					vm.speed=(byte)trackBar1.Value;
					error=vm.ccVM();
					break;
				case eCSTATE.STEP:
					textBox1.Text="last step\r\n";
					displayVMState(vm.pc);
					vm.speed=1;
					error=vm.ccVM();
					conState=eCSTATE.PAUSED;
					SetMenuState();
					break;
				default:
					error=vm_error.VM_OK;
					break;
			}
			switch (error)
			{
				case vm_error.VM_OK:
					break;
				case vm_error.VM_BREAK:
					textBox1.Text="break point reached:\r\n";
					conState=eCSTATE.PAUSED;
					SetMenuState();
					displayVMState(vm.pc);
					break;
				case vm_error.VM_END:
					textBox1.Text="game termnated with no error";
					conState=eCSTATE.STOPPED;
					SetMenuState();
					vm.Reset();
					break;
				default:
					textBox1.Text=String.Format("VM error:{0:S}\r\n",error_msg[(int)error]);
					displayVMState((ushort)(vm.pc-2));
					conState=eCSTATE.STOPPED;
					vm.Reset();
					break;
			}

		}


		void MainFormPaint(object sender, PaintEventArgs e)
		{
			
		}


		void RestartMenuItemClick(object sender, EventArgs e)
		{
			if (vm.codeInStore)
			{
				vm.Reset();
				textBox1.Text="Restarted";
				conState=eCSTATE.RUNNING;
				SetMenuState();
			}
		}
	
		void TrackBar1KeyDown(object sender, KeyEventArgs e)
		{
			if (conState==eCSTATE.RUNNING ||
			    conState==eCSTATE.PAUSED) vm.kpad.hexKeyDown((byte)e.KeyValue);
		}

		void StepMenuItemClick(object sender, EventArgs e)
		{
			conState=eCSTATE.STEP;
			SetMenuState();
		}
		
		void TrackBar1KeyUp(object sender, KeyEventArgs e)
		{
			if (conState==eCSTATE.RUNNING ||
			      conState==eCSTATE.PAUSED) vm.kpad.hexKeyUp((byte)e.KeyValue);
		}

		void AboutMenuItemClick(object sender, EventArgs e)
		{
			FormAbout about=new FormAbout();
			about.Show();
		}

		void PauseToolBtnClick(object sender, EventArgs e)
		{
			textBox1.Text="Paused, last instruction:\r\n";
			displayVMState((ushort)(vm.pc-2));
			conState=eCSTATE.PAUSED;
			SetMenuState();
		}

		void StopToolBtnClick(object sender, EventArgs e)
		{
			textBox1.Text="Stopped\r\n";
			vm.Reset();
			conState=eCSTATE.STOPPED;
			SetMenuState();
		}

		void ResumeToolBtnClick(object sender, EventArgs e)
		{
			textBox1.Text="Resumed";
			conState=eCSTATE.RUNNING;
			SetMenuState();
		}

		void BreakPointMenuItemClick(object sender, EventArgs e)
		{
			
			BreaksForm.Show();
			
		}

		void StepToolBtnClick(object sender, EventArgs e)
		{
			textBox1.Text="Single step";
			conState=eCSTATE.STEP;
			SetMenuState();
		}
		void ClearBreakPointsMenuItemClick(object sender, EventArgs e)
		{
			BreaksForm.ClearList();
		}
		void LoadLabelsFileMenuItemClick(object sender, EventArgs e)
		{
            openFileDialog1.Filter = "CHIPcon labels|*.lbl|All Files (*.*)|*.*";
            openFileDialog1.FilterIndex = 1;
            openFileDialog1.Multiselect = false;
            reloadLastFileMenuItem.Enabled=false;
            DialogResult result=openFileDialog1.ShowDialog();
            if (result == DialogResult.OK)
            {
            	BreaksForm.LoadLabels(openFileDialog1.FileName);
            }
		}
		void ReloadLastFileMenuItemClick(object sender, EventArgs e)
		{
		    byte[] data;
		    
			System.IO.Stream fileStream = openFileDialog1.OpenFile();
            data= new byte[Math.Min(3584,fileStream.Length)];
            using (System.IO.BinaryReader reader = new System.IO.BinaryReader(fileStream))
            {
            	reader.Read(data,0,(int)data.Length);
            }
            fileStream.Close();
            vm.load(data);
            textBox1.Text="Running";
            conState=eCSTATE.RUNNING;
            SetMenuState();
            BreaksForm.ClearList();
            string lbl_file=openFileDialog1.FileName.Split(new char[]{'.'})[0]+".lbl";
            BreaksForm.LoadLabels(lbl_file);

		}


	}
	

	
}
