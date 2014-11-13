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
 * Date: 2014-11-06
 * Time: 08:50
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
using System;
using System.Drawing;
using System.Windows.Forms;

namespace ccemul
{
	/// <summary>
	/// Description of Form1.
	/// </summary>
	public partial class FormAbout : Form
	{
		static String Version="1.0a";

		static String Info=String.Format(
		"ccemul version {0:S}\r\n"+
		"Copyright Jacques Deschênes (2014)\r\n"+
		"CHIPcon emulator\r\n"+
		"https://github.com/picatout/chipcon\r\n"+
		"\r\n"+
		"Project licence: GPLv3\r\n"
		,Version
		);
		
		public FormAbout()
		{
			//
			// The InitializeComponent() call is required for Windows Forms designer support.
			//
			InitializeComponent();
			
			//
			// TODO: Add constructor code after the InitializeComponent() call.
			//
		}
		void FormAboutLoad(object sender, EventArgs e)
		{
			this.AboutInfo.Text=Info;
		}
		void Button1Click(object sender, EventArgs e)
		{
			this.Close();
		}
	}
}
