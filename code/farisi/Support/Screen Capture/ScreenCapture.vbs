Set i1     = CreateObject("InitHolder") '''''''''''''''''''
Set o1     = CreateObject("Pipe")       ' try to activate '
Set txtWnd = CreateObject("ViewDesk")   '     objects     '
Set buff   = CreateObject("BufAccess")  '''''''''''''''''''

i1.SetFileName("Screen.ini")

o1.Init i1, "com"            '''''''''''''''''''
res = o1.Open                           '   try to open   '
If res = False Then                     '       port      '
    MsgBox "You have to start after host. Try again"      '
Else                                    'Go to end if err '
                                        '''''''''''''''''''
d = "FRAME_GRABBER"
txtWnd.Init i1, d
txtWnd.Write(d)

Const ForReading = 1, ForWriting = 2, ForAppending = 3
Const TristateUseDefault = -2, TristateTrue = -1, TristateFalse = 0
Dim fso, f, ts
Set fso = CreateObject("Scripting.FileSystemObject")

For a = 1 to 1000
	
	result = MsgBox("Capture?", 4)                      '      Pause      '
    
    If result = 7 Then
        Exit For
    End If
    
	HEXStr = "01"

    o1.ResetBuffer -1 ' all buffers reset
    
	buff.SetHEX o1, 1, 0, HEXStr
	o1.Write 1, Len(HEXStr)/2, 0
	txtWnd.ShowColoredTrace o1, 1, 0, Len(HEXStr)/2, "Request", 1

	Repeat = 0
	SizeRes = 0
	ByteReceived = 0
    Do
        If txtWnd.IsClosed = True Then Exit For  ' Check Exit button  '

        If SizeRes = 0 Then                      ''''''''''''''''''''''
           SizeRes = o1.Read(5, 4048, 100, 0)    '    Read message    '
        Else                                     '     from port      '
            ByteReceived = o1.Read(5, 4048-SizeRes, 100, SizeRes)     '
            SizeRes = SizeRes + ByteReceived     '                    '
            If ByteReceived = 0 Then             '                    '
               Repeat = Repeat + 1               '                    '
            End If                               '                    '
        End If                                   ''''''''''''''''''''''
    Loop Until Repeat > 3

	txtWnd.ShowColoredTrace o1, 5, 0, SizeRes, "Response", 2
	buff.CopyEx o1, 0, 0, o1, 5, 4, 1024
'	txtWnd.ShowColoredTrace o1, 0, 0, SizeRes, "picture", 2

    For j = 7 To 0 Step -1 ' rows
        For k = 0 To 127   ' 127 bytes in row
            For m = 0 To 7 ' 8 bits
                If GetBit(j*128 + k, m) = 0 Then
                    SetBit (7-j)*128 + (7-m)*16 + k\8, 7 - ((7-j)*128) mod 8 - k mod 8
                End If
            Next
        Next
    Next
    
	txtWnd.ShowColoredTrace o1, 1, 0, 1024, "Response", 2
	Header = "42 4D 3E 04 00 00 00 00 00 00 3E 00 00 00 28 00 00 00 80 00 00 00 40 00 00 00 01 00 01 00 00 00 00 00 00 04 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 FF FF FF 00 "
	buff.SetHEX o1, 2, 0, Header
	buff.CopyEx o1, 2, Len(Header)/3, o1, 1, 0, 1024
	txtWnd.ShowColoredTrace o1, 2, 0, Len(Header)/3+1024, "Response", 2
	
	FileName = "picture" + CStr(a) + ".bmp"
    fso.CreateTextFile FileName            'Create a file
    Set f = fso.GetFile(FileName)
    Set ts = f.OpenAsTextStream(ForWriting, TristateUseDefault)

	For i = 0 To Len(Header)/3+1023
        ts.Write Chr(o1.GetByte(2, i))
	Next
    ts.Close
    
Next

MsgBox "End"

End If

Function GetBit(ByteVal, BitVal)
'    txtWnd.Write "GetBit from " + CStr(ByteVal) + " " + CStr(BitVal)
    b = o1.GetByte(0, ByteVal)
    GetBit = Sgn(b And (2^BitVal))
End Function

Function SetBit(ByteVal, BitVal)
'    txtWnd.Write "SetBit to   " + CStr(ByteVal) + " " + CStr(BitVal)
    o1.SetByte 1, ByteVal, (o1.GetByte(1, ByteVal) or 2^BitVal)
End Function