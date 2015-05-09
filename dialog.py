import sys
import wx
import os

class AddBallDialog(wx.Dialog):
	
	def __init__(self, *args, **kw):
		super(AddBallDialog, self).__init__(*args, **kw) 
			
		self.InitUI()
		self.SetSize((300, 250))
		self.SetTitle("Add ball dialog")
		self.stream = os.fdopen(int(sys.argv[1]), 'w')
		self.ShowModal()
#		self.Destroy()
		
	def OK(self, e):
		msg = str(1)
		msg += '\n'+self.x_coord.GetValue()
		msg += '\n'+self.y_coord.GetValue()
		msg += '\n'+self.z_coord.GetValue()
		msg += '\n'+self.x_velocity.GetValue()
		msg += '\n'+self.y_velocity.GetValue()
		msg += '\n'+self.z_velocity.GetValue()
		msg += '\n'+self.x_av.GetValue()
		msg += '\n'+self.y_av.GetValue()
		msg += '\n'+self.z_av.GetValue()
		msg += '\n'
		self.stream.write(msg)
		self.stream.close()
		self.Close()
		
	def Cancel(self, e):
		self.stream.write(str(0)+'\n')
		self.stream.close()
		self.Close()
		
	def InitUI(self):

		vbox = wx.BoxSizer(wx.VERTICAL)
	
		hbox1 = wx.BoxSizer(wx.HORIZONTAL)  
		self.x_coord = wx.TextCtrl(self)
		hbox1.Add(self.x_coord)
		self.y_coord = wx.TextCtrl(self)
		hbox1.Add(self.y_coord)
		self.z_coord = wx.TextCtrl(self)
		hbox1.Add(self.z_coord)
	
		hbox2 = wx.BoxSizer(wx.HORIZONTAL)  
		self.x_velocity = wx.TextCtrl(self)
		hbox2.Add(self.x_velocity)
		self.y_velocity = wx.TextCtrl(self)
		hbox2.Add(self.y_velocity)
		self.z_velocity = wx.TextCtrl(self)
		hbox2.Add(self.z_velocity)
		
		hbox3 = wx.BoxSizer(wx.HORIZONTAL)  
		self.x_av = wx.TextCtrl(self)
		hbox3.Add(self.x_av)
		self.y_av = wx.TextCtrl(self)
		hbox3.Add(self.y_av)
		self.z_av = wx.TextCtrl(self)
		hbox3.Add(self.z_av)
	   
		hbox = wx.BoxSizer(wx.HORIZONTAL)
		okButton = wx.Button(self, label='Ok')
		closeButton = wx.Button(self, label='Close')
		hbox.Add(okButton)
		hbox.Add(closeButton, flag=wx.LEFT, border=5)

		vbox.Add(hbox1, proportion=1, flag=wx.ALIGN_CENTER|wx.TOP|wx.BOTTOM, border=10)
		vbox.Add(hbox2, proportion=1, flag=wx.ALIGN_CENTER|wx.TOP|wx.BOTTOM, border=10)
		vbox.Add(hbox3, proportion=1, flag=wx.ALIGN_CENTER|wx.TOP|wx.BOTTOM, border=10)
		vbox.Add(hbox, proportion=1, flag=wx.ALIGN_CENTER|wx.TOP|wx.BOTTOM, border=10)
		self.SetSizer(vbox)
		
		okButton.Bind(wx.EVT_BUTTON, self.OK)
		closeButton.Bind(wx.EVT_BUTTON, self.Cancel)
		
		
			  


def main():
	
	ex = wx.App()
	balldialog = AddBallDialog(None)
	ex.MainLoop()	


if __name__ == '__main__':
	main()
