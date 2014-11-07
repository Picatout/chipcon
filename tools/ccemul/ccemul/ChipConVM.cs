/*
 * Created by SharpDevelop.
 * User: Jacques
 * Date: 2014-11-06
 * Time: 17:06
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
using System;

namespace ccemul
{
	/// <summary>
	/// Description of ChipCon.
	/// </summary>
	internal class ChipConVM
	{
		const int ORG=512;
		internal TVout tv;
		Random rnd=new Random();
		internal byte speed;
		
		internal ushort dt; // durée délais jeux
		internal ushort st; // durée tonalité
		ushort pc; // compteur ordinal
		ushort sp; // pointeur de pile
		ushort ix; // registre pointeur de donnée
		ushort[] Stack=new ushort[32]; //pile des retours
		byte[] var=new byte[16];  // variables
		byte[] rpl=new byte[16];  // sauvegarde des variables
		ushort[] code= new ushort[4096]; // espace de code programme.

		internal ChipConVM()
		{
			sp=0;
			pc=ORG;
			dt=0;
			st=0;
		}
		
		internal void Run()
		{
			
			while (speed>0)
			{
				int x=rnd.Next()&127;
				int y=rnd.Next()&63;
				tv.plot(x,y,eOP.eINVERT);
				speed--;
	
			}
			
		}
		
		internal void Reset()
		{
			sp=0;
			pc=0;
			tv.cls();
		}
		
	}
}
