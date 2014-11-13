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
 * Date: 2014-11-07
 * Time: 14:34
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
using System;
using System.Runtime.InteropServices;
using System.Threading;

namespace ccemul
{
	/// <summary>
	/// Description of tone.
	/// </summary>
	public class Tone
	{
		[DllImport("kernel32.dll", SetLastError=true)]
    	static extern bool Beep(uint dwFreq, uint dwDuration);
		
    	ushort[]SCALE=new ushort[16]
    		{523,554,587,622,659,698,740,784,831,880,932,988,1047,1109,1175,1245};
    	
    	//Thread toneThread;
    	
    	uint freq, duration;
    	Thread toneThread;
    	
    	internal Tone()
		{
			
    	}
	
    	void beepThreadWrapper()
    	{
    		Beep(freq, duration);
    	}
    	
    	internal void play_tone(uint freq, uint duration, bool wait_end)
    	{
    		if (toneThread!=null){
    			toneThread.Abort();
    			while (toneThread.IsAlive);
    		}
    		{
    			toneThread = new Thread(new ThreadStart(beepThreadWrapper));
    		}
    		this.freq = freq;
    		this.duration=(uint) Math.Truncate(duration*16.67);
    		toneThread.Start();
    		if (wait_end)
    		{
    			toneThread.Join();
    		}
    		
    	}
		
    	internal void key_tone(uint k, uint duration,bool wait_end)
    	{
    		play_tone(SCALE[k],duration, wait_end);
    		
    	}
	}
}
