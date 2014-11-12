/*
 * Created by SharpDevelop.
 * User: Jacques
 * Date: 2014-11-11
 * Time: 17:21
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
using System;
using System.Drawing;
using System.Windows.Forms;

namespace ccemul
{
	/// <summary>
	/// Description of FormBreakPoints.
	/// </summary>
	public partial class FormBreakPoints : Form
	{
		MainForm mf;
		
		public FormBreakPoints(MainForm Main)
		{
			//
			// The InitializeComponent() call is required for Windows Forms designer support.
			//
			InitializeComponent();
			mf=Main;
			//
			// TODO: Add constructor code after the InitializeComponent() call.
			//
		}
		
		void CloseBtnClick(object sender, EventArgs e)
		{
			this.Visible=false;
		}
		
		void AddBtnClick(object sender, EventArgs e)
		{
			BreaksCombo.Items.Add(BreaksCombo.Text);
			mf.vm.AddBreakPoint((ushort)Convert.ToInt32(BreaksCombo.Text));
		}
		
		void DeleteBtnClick(object sender, EventArgs e)
		{
			mf.vm.RemoveBreakPoint((ushort)Convert.ToInt32(BreaksCombo.Text));
			BreaksCombo.Items.Remove(BreaksCombo.Text);
		}
		
		internal void ClearList()
		{
			BreaksCombo.Items.Clear();
			mf.vm.ClearBreakPoints();

		}
		
		internal void LoadLabels(String label_file)
		{
			
		}
		void LabelsListBoxSelectedIndexChanged(object sender, EventArgs e)
		{
			
		}
	}
}
