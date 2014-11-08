﻿/*
 * Created by SharpDevelop.
 * User: Jacques Deschênes
 * Date: 2014-11-06
 * Time: 15:54
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
using System;
using System.Drawing;

namespace ccemul
{
	/// <summary>
	/// contrôle l'affichage de l'émulateur
	/// </summary>


	internal class TVout 
	{
		
		internal const int PIXEL_SIZE=4;
		internal const byte VRES=64;
		internal const byte HRES=128;		
			
		internal uint FrameCounter=0;
		
		Graphics g;
		
		internal Bitmap display;
		
		internal TVout()
		{
			display=new Bitmap(128*PIXEL_SIZE,64*PIXEL_SIZE);
			g= Graphics.FromImage(display);
		}
		
		internal void cls()
		{
			g.Clear(Color.Black);
		}
		
		
		internal void plot(int x, int y, eOP op)
		{
			
			x*=PIXEL_SIZE;
			y*=PIXEL_SIZE;
			Rectangle r= new Rectangle(x,y,PIXEL_SIZE,PIXEL_SIZE);
			Brush b;
			
			switch (op){
				case eOP.eWHITE:
					b =new SolidBrush(Color.White);
					g.FillRectangle(b,r);
					b.Dispose();
					break;
				case eOP.eBLACK:
					b =new SolidBrush(Color.White);
					g.FillRectangle(b,r);
					b.Dispose();
					break;
				case eOP.eINVERT:
					Color c=display.GetPixel(x,y);
					if (c.B==255 && c.R==255 && c.G==255)
					{   
						b=new SolidBrush(Color.Black);
						g.FillRectangle(b,r);
					}
					else
					{
						b=new SolidBrush(Color.White);
						g.FillRectangle(b,r);
					}
					b.Dispose();
					break;
			}
		
		}
		
		internal byte putSprite(int x, int y, int width, int height, byte[] sprite)
		{
			byte row;
			bool collision=false;
			int i=7;
			for (int y0=0;y0<height;y0++)
			{
				if (y+y0<0 || y+y0>=VRES) continue;
				for (int x0=0;x0<width;x0++)
				{
					if (x+x0<0 || x+x0>=HRES) continue;
					row=sprite[(y0*width+x0)/8];
					if ((row&(1<<i))==(1<<i))
					{
						plot(x+x0,y+y0,eOP.eINVERT);
						collision=true;
					}
					i--;
					if (i<0) i=7;
					
				}
			}
			if (collision) return 1; else return 0;
		}
		
		internal void scrollDown(int n)
		{
		    Rectangle r = new Rectangle(0, 0,HRES*PIXEL_SIZE,(VRES-n)*PIXEL_SIZE);
		    System.Drawing.Imaging.PixelFormat f = display.PixelFormat;
		    Bitmap tmpBmp = display.Clone(r,f);
		    g.Clear(Color.Black);
		    g.DrawImage(tmpBmp,new Point(0,n*PIXEL_SIZE));
		    tmpBmp.Dispose();
		}
		
		internal void scrollUp(int n)
		{
		    Rectangle r = new Rectangle(0,n*PIXEL_SIZE,HRES*PIXEL_SIZE,(VRES-n)*PIXEL_SIZE);
		    System.Drawing.Imaging.PixelFormat f = display.PixelFormat;
		    Bitmap tmpBmp = display.Clone(r,f);
		    g.Clear(Color.Black);
		    g.DrawImage(tmpBmp,new Point(0,0));
		    tmpBmp.Dispose();
		}
		
		internal void scrollLeft(int n)
		{
			Rectangle r = new Rectangle(n*PIXEL_SIZE,0,(HRES-n)*PIXEL_SIZE,VRES*PIXEL_SIZE);
		    System.Drawing.Imaging.PixelFormat f = display.PixelFormat;
		    Bitmap tmpBmp = display.Clone(r,f);
		    g.Clear(Color.Black);
		    g.DrawImage(tmpBmp,new Point(0,0));
		    tmpBmp.Dispose();
		}
		
		internal void scrollRight(int n)
		{
			Rectangle r = new Rectangle(0,0,(HRES-n)*PIXEL_SIZE,VRES*PIXEL_SIZE);
		    System.Drawing.Imaging.PixelFormat f = display.PixelFormat;
		    Bitmap tmpBmp = display.Clone(r,f);
		    g.Clear(Color.Black);
		    g.DrawImage(tmpBmp,new Point(n*PIXEL_SIZE,0));
		    tmpBmp.Dispose();
		}
		
	}
}