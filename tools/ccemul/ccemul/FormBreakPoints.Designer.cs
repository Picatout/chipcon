/*
 * Created by SharpDevelop.
 * User: Jacques
 * Date: 2014-11-11
 * Time: 17:21
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
namespace ccemul
{
	partial class FormBreakPoints
	{
		/// <summary>
		/// Designer variable used to keep track of non-visual components.
		/// </summary>
		private System.ComponentModel.IContainer components = null;
		private System.Windows.Forms.ComboBox BreaksCombo;
		private System.Windows.Forms.Button AddBtn;
		private System.Windows.Forms.Button DeleteBtn;
		private System.Windows.Forms.Button CloseBtn;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.ListBox LabelsListBox;
		private System.Windows.Forms.Label label2;
		
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
			this.BreaksCombo = new System.Windows.Forms.ComboBox();
			this.AddBtn = new System.Windows.Forms.Button();
			this.DeleteBtn = new System.Windows.Forms.Button();
			this.CloseBtn = new System.Windows.Forms.Button();
			this.label1 = new System.Windows.Forms.Label();
			this.LabelsListBox = new System.Windows.Forms.ListBox();
			this.label2 = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// BreaksCombo
			// 
			this.BreaksCombo.FormattingEnabled = true;
			this.BreaksCombo.Location = new System.Drawing.Point(12, 225);
			this.BreaksCombo.MaxDropDownItems = 10;
			this.BreaksCombo.Name = "BreaksCombo";
			this.BreaksCombo.Size = new System.Drawing.Size(171, 21);
			this.BreaksCombo.Sorted = true;
			this.BreaksCombo.TabIndex = 0;
			// 
			// AddBtn
			// 
			this.AddBtn.Location = new System.Drawing.Point(189, 225);
			this.AddBtn.Name = "AddBtn";
			this.AddBtn.Size = new System.Drawing.Size(75, 23);
			this.AddBtn.TabIndex = 1;
			this.AddBtn.Text = "Add";
			this.AddBtn.UseVisualStyleBackColor = true;
			this.AddBtn.Click += new System.EventHandler(this.AddBtnClick);
			// 
			// DeleteBtn
			// 
			this.DeleteBtn.Location = new System.Drawing.Point(189, 276);
			this.DeleteBtn.Name = "DeleteBtn";
			this.DeleteBtn.Size = new System.Drawing.Size(75, 23);
			this.DeleteBtn.TabIndex = 2;
			this.DeleteBtn.Text = "Delete";
			this.DeleteBtn.UseVisualStyleBackColor = true;
			this.DeleteBtn.Click += new System.EventHandler(this.DeleteBtnClick);
			// 
			// CloseBtn
			// 
			this.CloseBtn.Location = new System.Drawing.Point(189, 320);
			this.CloseBtn.Name = "CloseBtn";
			this.CloseBtn.Size = new System.Drawing.Size(75, 23);
			this.CloseBtn.TabIndex = 3;
			this.CloseBtn.Text = "Close";
			this.CloseBtn.UseVisualStyleBackColor = true;
			this.CloseBtn.Click += new System.EventHandler(this.CloseBtnClick);
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(12, 199);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(143, 23);
			this.label1.TabIndex = 4;
			this.label1.Text = "actives break points";
			// 
			// LabelsListBox
			// 
			this.LabelsListBox.FormattingEnabled = true;
			this.LabelsListBox.Location = new System.Drawing.Point(12, 38);
			this.LabelsListBox.Name = "LabelsListBox";
			this.LabelsListBox.Size = new System.Drawing.Size(252, 134);
			this.LabelsListBox.TabIndex = 5;
			this.LabelsListBox.SelectedIndexChanged += new System.EventHandler(this.LabelsListBoxSelectedIndexChanged);
			// 
			// label2
			// 
			this.label2.Location = new System.Drawing.Point(12, 12);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(171, 23);
			this.label2.TabIndex = 6;
			this.label2.Text = "Available Labels";
			// 
			// FormBreakPoints
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(292, 385);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.LabelsListBox);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.CloseBtn);
			this.Controls.Add(this.DeleteBtn);
			this.Controls.Add(this.AddBtn);
			this.Controls.Add(this.BreaksCombo);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "FormBreakPoints";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Break points";
			this.ResumeLayout(false);

		}
	}
}
