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
 * Date: 2014-11-11
 * Time: 17:21
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
using System;
using System.IO;
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
			mf.vm.AddBreakPoint((ushort)Convert.ToInt32(BreaksCombo.Text.Split(new char[]{' ','\t'})[1]));
		}
		
		void DeleteBtnClick(object sender, EventArgs e)
		{
			mf.vm.RemoveBreakPoint((ushort)Convert.ToInt32(BreaksCombo.Text.Split(new char[]{' ','\t'})[1]));
			BreaksCombo.Items.Remove(BreaksCombo.Text);
		}
		
		internal void ClearList()
		{
			BreaksCombo.Items.Clear();
			mf.vm.ClearBreakPoints();

		}
		
		internal void LoadLabels(string textFile)
		{
			string line;
		  	
			try{
				StreamReader sr= new StreamReader(textFile);
				line=sr.ReadLine();
				while (line!=null)
				{
	            	LabelsListBox.Items.Add(line);
	            	line=sr.ReadLine();
					
				}
				sr.Close();
			}
			catch (Exception e)
			{
				MessageBox.Show(string.Format("Loading labels file failed!\r\n {0}",e.Message));
				
			}
		}
		
		void LabelsListBoxSelectedIndexChanged(object sender, EventArgs e)
		{
			BreaksCombo.Text=(string)LabelsListBox.SelectedItem;
		}
		
	
		
	}
}
