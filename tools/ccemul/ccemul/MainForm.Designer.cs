/*
 * Created by SharpDevelop.
 * User: Jacques
 * Date: 2014-11-05
 * Time: 08:27
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
namespace ccemul
{
	partial class MainForm
	{
		/// <summary>
		/// Designer variable used to keep track of non-visual components.
		/// </summary>
		private System.ComponentModel.IContainer components = null;
		
		/// <summary>
		/// Disposes resources used by the form.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing) {
				if (components != null) {
					components.Dispose();
				}
			}
			base.Dispose(disposing);
		}
		
		/// <summary>
		/// This method is required for Windows Forms designer support.
		/// Do not change the method contents inside the source code editor. The Forms designer might
		/// not be able to load this method if it was changed manually.
		/// </summary>
		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
			this.menuStrip1 = new System.Windows.Forms.MenuStrip();
			this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
			this.openFileMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.reloadLastFileMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.RestartMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
			this.quitMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.MenuDebug = new System.Windows.Forms.ToolStripMenuItem();
			this.BreakPointMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.clearBreakPointsMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.loadLabelsFileMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.MenuHelp = new System.Windows.Forms.ToolStripMenuItem();
			this.AboutMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
			this.timer1 = new System.Windows.Forms.Timer(this.components);
			this.trackBar1 = new System.Windows.Forms.TrackBar();
			this.pictureBox1 = new System.Windows.Forms.PictureBox();
			this.textBox1 = new System.Windows.Forms.TextBox();
			this.label1 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.toolStrip1 = new System.Windows.Forms.ToolStrip();
			this.RestartToolBtn = new System.Windows.Forms.ToolStripButton();
			this.PauseToolBtn = new System.Windows.Forms.ToolStripButton();
			this.ResumeToolBtn = new System.Windows.Forms.ToolStripButton();
			this.StepToolBtn = new System.Windows.Forms.ToolStripButton();
			this.StopToolBtn = new System.Windows.Forms.ToolStripButton();
			this.menuStrip1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.trackBar1)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
			this.toolStrip1.SuspendLayout();
			this.SuspendLayout();
			// 
			// menuStrip1
			// 
			this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
			this.toolStripMenuItem1,
			this.MenuDebug,
			this.MenuHelp});
			this.menuStrip1.Location = new System.Drawing.Point(0, 0);
			this.menuStrip1.Name = "menuStrip1";
			this.menuStrip1.Size = new System.Drawing.Size(638, 24);
			this.menuStrip1.TabIndex = 0;
			this.menuStrip1.Text = "menuStrip1";
			// 
			// toolStripMenuItem1
			// 
			this.toolStripMenuItem1.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
			this.openFileMenuItem,
			this.reloadLastFileMenuItem,
			this.RestartMenuItem,
			this.toolStripSeparator1,
			this.quitMenuItem});
			this.toolStripMenuItem1.Name = "toolStripMenuItem1";
			this.toolStripMenuItem1.Size = new System.Drawing.Size(40, 20);
			this.toolStripMenuItem1.Text = "Files";
			this.toolStripMenuItem1.Click += new System.EventHandler(this.ToolStripMenuItem1Click);
			// 
			// openFileMenuItem
			// 
			this.openFileMenuItem.Name = "openFileMenuItem";
			this.openFileMenuItem.Size = new System.Drawing.Size(152, 22);
			this.openFileMenuItem.Text = "Open...";
			this.openFileMenuItem.Click += new System.EventHandler(this.OpenFileMenuItemClick);
			// 
			// reloadLastFileMenuItem
			// 
			this.reloadLastFileMenuItem.Enabled = false;
			this.reloadLastFileMenuItem.Name = "reloadLastFileMenuItem";
			this.reloadLastFileMenuItem.Size = new System.Drawing.Size(152, 22);
			this.reloadLastFileMenuItem.Text = "Reload last file";
			this.reloadLastFileMenuItem.Click += new System.EventHandler(this.ReloadLastFileMenuItemClick);
			// 
			// RestartMenuItem
			// 
			this.RestartMenuItem.Name = "RestartMenuItem";
			this.RestartMenuItem.Size = new System.Drawing.Size(152, 22);
			this.RestartMenuItem.Text = "Restart";
			this.RestartMenuItem.Click += new System.EventHandler(this.RestartMenuItemClick);
			// 
			// toolStripSeparator1
			// 
			this.toolStripSeparator1.Name = "toolStripSeparator1";
			this.toolStripSeparator1.Size = new System.Drawing.Size(149, 6);
			// 
			// quitMenuItem
			// 
			this.quitMenuItem.Name = "quitMenuItem";
			this.quitMenuItem.Size = new System.Drawing.Size(152, 22);
			this.quitMenuItem.Text = "Quit";
			this.quitMenuItem.Click += new System.EventHandler(this.QuitMenuItemClick);
			// 
			// MenuDebug
			// 
			this.MenuDebug.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
			this.BreakPointMenuItem,
			this.clearBreakPointsMenuItem,
			this.loadLabelsFileMenuItem});
			this.MenuDebug.Name = "MenuDebug";
			this.MenuDebug.Size = new System.Drawing.Size(49, 20);
			this.MenuDebug.Text = "debug";
			// 
			// BreakPointMenuItem
			// 
			this.BreakPointMenuItem.Name = "BreakPointMenuItem";
			this.BreakPointMenuItem.Size = new System.Drawing.Size(161, 22);
			this.BreakPointMenuItem.Text = "Break point...";
			this.BreakPointMenuItem.Click += new System.EventHandler(this.BreakPointMenuItemClick);
			// 
			// clearBreakPointsMenuItem
			// 
			this.clearBreakPointsMenuItem.Name = "clearBreakPointsMenuItem";
			this.clearBreakPointsMenuItem.Size = new System.Drawing.Size(161, 22);
			this.clearBreakPointsMenuItem.Text = "Clear break points";
			this.clearBreakPointsMenuItem.Click += new System.EventHandler(this.ClearBreakPointsMenuItemClick);
			// 
			// loadLabelsFileMenuItem
			// 
			this.loadLabelsFileMenuItem.Name = "loadLabelsFileMenuItem";
			this.loadLabelsFileMenuItem.Size = new System.Drawing.Size(161, 22);
			this.loadLabelsFileMenuItem.Text = "Load labels file";
			this.loadLabelsFileMenuItem.Click += new System.EventHandler(this.LoadLabelsFileMenuItemClick);
			// 
			// MenuHelp
			// 
			this.MenuHelp.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
			this.AboutMenuItem});
			this.MenuHelp.Name = "MenuHelp";
			this.MenuHelp.Size = new System.Drawing.Size(39, 20);
			this.MenuHelp.Text = "help";
			// 
			// AboutMenuItem
			// 
			this.AboutMenuItem.Name = "AboutMenuItem";
			this.AboutMenuItem.Size = new System.Drawing.Size(115, 22);
			this.AboutMenuItem.Text = "About...";
			this.AboutMenuItem.Click += new System.EventHandler(this.AboutMenuItemClick);
			// 
			// openFileDialog1
			// 
			this.openFileDialog1.FileName = "openFileDialog1";
			// 
			// timer1
			// 
			this.timer1.Interval = 16;
			this.timer1.Tick += new System.EventHandler(this.Timer1Tick);
			// 
			// trackBar1
			// 
			this.trackBar1.Location = new System.Drawing.Point(12, 106);
			this.trackBar1.Maximum = 255;
			this.trackBar1.Minimum = 5;
			this.trackBar1.Name = "trackBar1";
			this.trackBar1.Orientation = System.Windows.Forms.Orientation.Vertical;
			this.trackBar1.Size = new System.Drawing.Size(42, 269);
			this.trackBar1.TabIndex = 3;
			this.trackBar1.TickFrequency = 10;
			this.trackBar1.Value = 50;
			this.trackBar1.KeyDown += new System.Windows.Forms.KeyEventHandler(this.TrackBar1KeyDown);
			this.trackBar1.KeyUp += new System.Windows.Forms.KeyEventHandler(this.TrackBar1KeyUp);
			// 
			// pictureBox1
			// 
			this.pictureBox1.InitialImage = null;
			this.pictureBox1.Location = new System.Drawing.Point(64, 106);
			this.pictureBox1.Name = "pictureBox1";
			this.pictureBox1.Size = new System.Drawing.Size(562, 269);
			this.pictureBox1.TabIndex = 4;
			this.pictureBox1.TabStop = false;
			// 
			// textBox1
			// 
			this.textBox1.Enabled = false;
			this.textBox1.Location = new System.Drawing.Point(12, 427);
			this.textBox1.Multiline = true;
			this.textBox1.Name = "textBox1";
			this.textBox1.ReadOnly = true;
			this.textBox1.Size = new System.Drawing.Size(614, 117);
			this.textBox1.TabIndex = 5;
			this.textBox1.TabStop = false;
			// 
			// label1
			// 
			this.label1.Enabled = false;
			this.label1.Location = new System.Drawing.Point(12, 86);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(42, 17);
			this.label1.TabIndex = 6;
			this.label1.Text = "speed";
			// 
			// label2
			// 
			this.label2.Enabled = false;
			this.label2.Location = new System.Drawing.Point(258, 392);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(100, 23);
			this.label2.TabIndex = 7;
			this.label2.Text = "debug messages";
			// 
			// toolStrip1
			// 
			this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
			this.RestartToolBtn,
			this.PauseToolBtn,
			this.ResumeToolBtn,
			this.StepToolBtn,
			this.StopToolBtn});
			this.toolStrip1.Location = new System.Drawing.Point(0, 24);
			this.toolStrip1.Name = "toolStrip1";
			this.toolStrip1.Size = new System.Drawing.Size(638, 25);
			this.toolStrip1.TabIndex = 8;
			this.toolStrip1.Text = "toolStrip1";
			// 
			// RestartToolBtn
			// 
			this.RestartToolBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.RestartToolBtn.Enabled = false;
			this.RestartToolBtn.Image = ((System.Drawing.Image)(resources.GetObject("RestartToolBtn.Image")));
			this.RestartToolBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.RestartToolBtn.Name = "RestartToolBtn";
			this.RestartToolBtn.Size = new System.Drawing.Size(23, 22);
			this.RestartToolBtn.Text = "Restart";
			this.RestartToolBtn.Click += new System.EventHandler(this.RestartMenuItemClick);
			// 
			// PauseToolBtn
			// 
			this.PauseToolBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.PauseToolBtn.Enabled = false;
			this.PauseToolBtn.Image = ((System.Drawing.Image)(resources.GetObject("PauseToolBtn.Image")));
			this.PauseToolBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.PauseToolBtn.Name = "PauseToolBtn";
			this.PauseToolBtn.Size = new System.Drawing.Size(23, 22);
			this.PauseToolBtn.Text = "Pause";
			this.PauseToolBtn.Click += new System.EventHandler(this.PauseToolBtnClick);
			// 
			// ResumeToolBtn
			// 
			this.ResumeToolBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.ResumeToolBtn.Enabled = false;
			this.ResumeToolBtn.Image = ((System.Drawing.Image)(resources.GetObject("ResumeToolBtn.Image")));
			this.ResumeToolBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.ResumeToolBtn.Name = "ResumeToolBtn";
			this.ResumeToolBtn.Size = new System.Drawing.Size(23, 22);
			this.ResumeToolBtn.Text = "Resume";
			this.ResumeToolBtn.Click += new System.EventHandler(this.ResumeToolBtnClick);
			// 
			// StepToolBtn
			// 
			this.StepToolBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.StepToolBtn.Enabled = false;
			this.StepToolBtn.Image = ((System.Drawing.Image)(resources.GetObject("StepToolBtn.Image")));
			this.StepToolBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.StepToolBtn.Name = "StepToolBtn";
			this.StepToolBtn.Size = new System.Drawing.Size(23, 22);
			this.StepToolBtn.Text = "step";
			this.StepToolBtn.Click += new System.EventHandler(this.StepToolBtnClick);
			// 
			// StopToolBtn
			// 
			this.StopToolBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.StopToolBtn.Enabled = false;
			this.StopToolBtn.Image = ((System.Drawing.Image)(resources.GetObject("StopToolBtn.Image")));
			this.StopToolBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.StopToolBtn.Name = "StopToolBtn";
			this.StopToolBtn.Size = new System.Drawing.Size(23, 22);
			this.StopToolBtn.Text = "Stop";
			this.StopToolBtn.Click += new System.EventHandler(this.StopToolBtnClick);
			// 
			// MainForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(638, 556);
			this.Controls.Add(this.toolStrip1);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.textBox1);
			this.Controls.Add(this.pictureBox1);
			this.Controls.Add(this.trackBar1);
			this.Controls.Add(this.menuStrip1);
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MainMenuStrip = this.menuStrip1;
			this.MaximizeBox = false;
			this.Name = "MainForm";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "CHIPcon emulator";
			this.Shown += new System.EventHandler(this.MainFormShown);
			this.Paint += new System.Windows.Forms.PaintEventHandler(this.MainFormPaint);
			this.menuStrip1.ResumeLayout(false);
			this.menuStrip1.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.trackBar1)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
			this.toolStrip1.ResumeLayout(false);
			this.toolStrip1.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}
		private System.Windows.Forms.PictureBox pictureBox1;
		private System.Windows.Forms.TrackBar trackBar1;
		private System.Windows.Forms.Timer timer1;
		private System.Windows.Forms.OpenFileDialog openFileDialog1;
		private System.Windows.Forms.ToolStripMenuItem quitMenuItem;
		private System.Windows.Forms.ToolStripMenuItem RestartMenuItem;
		private System.Windows.Forms.ToolStripMenuItem openFileMenuItem;
		private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem1;
		private System.Windows.Forms.MenuStrip menuStrip1;
		private System.Windows.Forms.TextBox textBox1;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.ToolStripMenuItem MenuHelp;
		private System.Windows.Forms.ToolStripMenuItem AboutMenuItem;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.ToolStripMenuItem MenuDebug;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
		private System.Windows.Forms.ToolStripMenuItem BreakPointMenuItem;
		private System.Windows.Forms.ToolStrip toolStrip1;
	
		private System.Windows.Forms.ToolStripButton PauseToolBtn;
		private System.Windows.Forms.ToolStripButton StopToolBtn;
		private System.Windows.Forms.ToolStripButton ResumeToolBtn;
		private System.Windows.Forms.ToolStripButton StepToolBtn;
		private System.Windows.Forms.ToolStripButton RestartToolBtn;
		private System.Windows.Forms.ToolStripMenuItem clearBreakPointsMenuItem;
		private System.Windows.Forms.ToolStripMenuItem loadLabelsFileMenuItem;
		private System.Windows.Forms.ToolStripMenuItem reloadLastFileMenuItem;
	}
}
