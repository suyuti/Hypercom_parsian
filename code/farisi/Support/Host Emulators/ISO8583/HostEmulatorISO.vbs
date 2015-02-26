Set i1 = CreateObject("InitHolder")                   ''''''''''''''''''''''
Set o1 = CreateObject("Pipe")                         '  try to activate   '
Set txtWnd = CreateObject("ViewDesk")                 '      objects       '
Set iso = CreateObject("ISOMessage")                  '                    '
Set buff   = CreateObject("BufAccess")                ''''''''''''''''''''''

Dim Red, Blue, Green, Violet, Orange, Grey
SetColors

i1.SetFileName("HostEmulator.ini")

host = "HOST"
txtWnd.Init i1, host

o1.Init i1, "com"

Connect = i1.GetString ("Pipe_com", "Port Type", "serial")
If Connect = "ip" Then 
    IsIp = True
Else 
    IsIp = False
End If

res = o1.Open                     '''''''''''''''''''''''''''''''''
If res = False Then               '                               '
    txtWnd.CloseDesk              '        try to open Port       '
    MsgBox o1.GetError            '         for connection        '
End If                            '''''''''''''''''''''''''''''''''

InputBuf = 0
OutputBuf = 1

iso.Init i1, "ISO"                ' get format and other params from ini-file

'iso.SetSettlement False          ' you can switch off settlement decoding
'iso.SetField63Decoded False      ' you can switch off Parsing 63 field

txtWnd.ColoredWrite host, Grey
txtWnd.ColoredWrite "Port: " + o1.InitString + " was opened", Grey

'txtWnd.IsLogToFile = False       ' you can switch off logging to file
'txtWnd.IsLogToScreen = False     ' you can switch off logging to screen

For a = 0 To 100000000

    Do until False                                    '   Endless loop     '
        iso.SetAutoDate True                          ' you have to switch on AutoSet Date and Time (Fields 12 and 13)
                                                      ' it may be switch off when decoding Adjust Sale
    	Repeat = 0
    	SizeReq = 0
    	ByteReceived = 0
        Do
            If txtWnd.IsClosed = True Then Exit For  ' Check Exit button  '

            If SizeReq = 0 Then                      ''''''''''''''''''''''
               SizeReq = o1.Read(InputBuf, 1024, 100, 0)'  Read message   '
            Else                                     '     from port      '
                ByteReceived = o1.Read(InputBuf, 1024-SizeReq, 100, SizeReq)
                SizeReq = SizeReq + ByteReceived     '                    '
                If ByteReceived = 0 Then             '                    '
                   Repeat = Repeat + 1               '                    '
                End If                               '                    '
            End If                                   ''''''''''''''''''''''
        Loop Until Repeat > 3
                                                                      ''''''''''''''''''''''
        If buff.CheckFrame(o1, InputBuf, SizeReq, IsIp) = False Then  '       Check        '
            txtWnd.ShowColoredTrace o1, InputBuf, 0, SizeReq, "Corrupted message was received", Red
        Else                                                          '      framing       '
            txtWnd.ShowColoredTrace o1, InputBuf, 0, SizeReq, "Request ", Violet           '
        End If                                                        ''''''''''''''''''''''
                                                                      ''''''''''''''''''''''
        CountFields = iso.Decode (o1, InputBuf, 8 + CInt(IsIp), SizeReq-8-CInt(IsIp))      '
        If CountFields = 0 Then                                       '                    '
            e = iso.GetError                                          '   Decode message   '
            txtWnd.Write(e)                                           '                    '
        End If                                                        ''''''''''''''''''''''

        Call CreateResponse   ' !!!!!!!!!!!!!!!!!!!!!!! MAIN SUBROUTINE !!!!!!!!!!!!!!!!!!!!!!!!!!!!

        txtWnd.ShowColoredMessage iso, True, Violet
	    CheckField55                                                  ' Check Mandatory Tags Field 55

        SizeRes = iso.Encode (o1, OutputBuf, 0)                       ''''''''''''''''''''''
        If SizeRes = 0 Then                                           '                    '
            e = iso.GetError                                          '   Encode message   '
            txtWnd.Write e                                            '                    '
            Exit Do                                                   '                    '
        End If                                                        ''''''''''''''''''''''
                                                                      ''''''''''''''''''''''
        SizeRes = buff.SwapTPDU(o1, OutputBuf, o1, InputBuf, SizeRes, IsIp)' Create TPDU   '
        SizeRes = buff.Frame(o1, OutputBuf, SizeRes, IsIp)            '   Create framing   '
                                                                      ''''''''''''''''''''''
        txtWnd.ShowColoredTrace o1, OutputBuf, 0, SizeRes, "Response", Blue

        CountFields = iso.Decode (o1, OutputBuf, 8 + CInt(IsIp), SizeRes-8-CInt(IsIp))
        txtWnd.ShowColoredMessage iso, True, Blue

        o1.Write OutputBuf, SizeRes, 0                                ' Send message to port

    Loop

Next


''''''''''''' CREATE RESPONSE ''''''''''''''''''

Sub CreateResponse

strMsgType = iso.GetField(0)
strProcessingCode = iso.GetField(3)

MsgErr = " not present. ERROR"

If (strMsgType = "0100" And Mid(strProcessingCode, 1, 2) = "00" And Mid(strProcessingCode, 4, 2) = "00") Then
    ColoredReport "Authorization", Violet
    Call Message
    'Addend to response 
    iso.SetField 0, "0110"                 ' MsgType
    iso.SetField 3, "000000"               ' ProcessingCode
    
ElseIf (strMsgType = "0100" And Mid(strProcessingCode, 1, 2) = "30" And Mid(strProcessingCode, 4, 2) = "00")Then
    ColoredReport "Pre-Authorization", Violet
    Call Message
    'Addend to response    
    iso.SetField 0, "0110"                 ' MsgType
    iso.SetField 3, "300000"               ' ProcessingCode

ElseIf (strMsgType = "0200" And Mid(strProcessingCode, 1, 2) = "00" And Mid(strProcessingCode, 4, 2) = "00")Then
    ColoredReport "Sale/Debit", Violet
    Call Message
    'Addend to response
    iso.SetField 0, "0210"                 ' MsgType
    iso.SetField 3, "000000"               ' ProcessingCode
    
ElseIf (strMsgType = "0200" And Mid(strProcessingCode, 1, 2) = "20" And Mid(strProcessingCode, 4, 2) = "00")Then
    ColoredReport "Refund", Violet
    Call Message    
    'Addend to response
    iso.SetField 0, "0210"                 ' MsgType
    iso.SetField 3, "200001"               ' ProcessingCode
    
ElseIf (strMsgType = "0200" And Mid(strProcessingCode, 1, 2) = "01" And Mid(strProcessingCode, 4, 2) = "00")Then
    ColoredReport "Cash", Violet
    Call Message
    iso.SetField 0, "0210"                 ' MsgType
    iso.SetField 3, "010000"               ' ProcessingCode
    
ElseIf (strMsgType = "0200" And Mid(strProcessingCode, 1, 2) = "09" And Mid(strProcessingCode, 4, 2) = "00")Then
    ColoredReport "Sale & Cash", Violet
    Call Message
    iso.SetField 0, "0210"                 ' MsgType
    iso.SetField 3, "090000"               ' ProcessingCode
ElseIf (strMsgType = "0200" And Mid(strProcessingCode, 1, 2) = "02" And Mid(strProcessingCode, 4, 2) = "00")Then
    ColoredReport "Void, Sale, on-line", Violet
    Call Message
    iso.SetField 0, "0210"                 ' MsgType
    iso.SetField 3, "020000"               ' ProcessingCode
ElseIf (strMsgType = "0220" And Mid(strProcessingCode, 1, 2) = "02" And Mid(strProcessingCode, 4, 2) = "00")Then
    ColoredReport "Adjust Sale", Violet
    iso.SetAutoDate False     ' you have to switch off AutoSet Date and Time (Fields 12 and 13)
    If (iso.GetField(38) = "") Then ColoredReport "Auth. Id. Response is not present. ERROR", Red
    iso.SetField 0, "0230"                 ' MsgType
    iso.SetField 3, "020000"               ' ProcessingCode
    ' Check mandatory fields
    If (iso.GetField(4) = "") Then ColoredReport "Amount not specified", Red
    strTraceNo = iso.GetField(11)
    If (strTraceNo = "") Then ColoredReport "System Trace Number" + MsgErr, Red
    strNII = iso.GetField(24)
    If (strNII = "") Then ColoredReport "NII" + MsgErr, Red
    strTerminalID = iso.GetField(41)
    If (strTerminalID = "") Then ColoredReport "Terminal ID" + MsgErr, Red
    If (iso.GetField(42) = "") Then ColoredReport "Acquirer ID" + MsgErr, Red

    ' Form the response
    iso.SetField 11, strTraceNo            ' TraceNo
    iso.SetField 24, strNII                ' NII
    iso.SetField 37, "000000000012"        ' RetRefNo
    iso.SetField 39, "00"                  ' ResponseCode
    iso.SetField 41, strTerminalID         ' TerminalID
ElseIf (strMsgType = "0220" And Mid(strProcessingCode, 1, 2) = "00" And Mid(strProcessingCode, 4, 2) = "00")Then
    ColoredReport "Off-Line Sale", Violet
    If (iso.GetField(38) = "") Then ColoredReport "Auth. Id. Response" + MsgErr, Red
    Call Message
    iso.SetField 0, "0230"                 ' MsgType
    iso.SetField 3, "000000"               ' ProcessingCode
ElseIf (strMsgType = "0220" And Mid(strProcessingCode, 1, 2) = "20" And Mid(strProcessingCode, 4, 2) = "00")Then
    ColoredReport "Off-Line Refund or Sales Completion", Violet
    If (iso.GetField(38) = "") Then ColoredReport "Auth. Id. Response" + MsgErr, Red
    Call Message
    iso.SetField 0, "0230"                 ' MsgType
    iso.SetField 3, "200000"               ' ProcessingCode

ElseIf (strMsgType = "0400" ) Then
    ColoredReport "Reversal", Violet
    If (iso.GetField(25) = "") Then ColoredReport "POSConditionCode" + MsgErr, Red
    Call Message    
    iso.SetField 0, "0410"                 ' MsgType
    iso.SetField 3, strProcessingCode      ' ProcessingCode

ElseIf (strMsgType = "0800" And Mid(strProcessingCode, 1, 2) = "99" And Mid(strProcessingCode, 4, 2) = "00")Then
    ColoredReport "Test Transaction", Violet
    strNII = iso.GetField(24)
    If (strNII = "") Then ColoredReport "NII" + MsgErr, Red
    strTerminalID = iso.GetField(41)
    If (strTerminalID = "") Then ColoredReport "Terminal ID" + MsgErr, Red

    strTraceNo = iso.GetField(11)
    
    iso.SetField 0, "0810"                 ' MsgType
    iso.SetField 3, "990000"               ' ProcessingCode
    iso.SetField 11, strTraceNo            ' TraceNo
    iso.SetField 24, strNII                ' NII
    iso.SetField 41, strTerminalID         ' TerminalID

ElseIf (strMsgType = "0500" And Mid(strProcessingCode, 1, 2) = "92" And Mid(strProcessingCode, 4, 2) = "00")Then
    ColoredReport "Settlement", Violet
    strTraceNo = iso.GetField(11)
    If (strTraceNo = "") Then ColoredReport "System Trace Number" + MsgErr, Red
    strNII = iso.GetField(24)
    If (strNII = "") Then ColoredReport "NII" + MsgErr, Red
    strTerminalID = iso.GetField(41)
    If (strTerminalID = "") Then ColoredReport "Terminal ID" + MsgErr, Red
    If (iso.GetField(42) = "") Then ColoredReport "Acquirer ID" + MsgErr, Red
    
    iso.SetField 0, "0510"                 ' MsgType
    iso.SetField 3, "920000"               ' ProcessingCode
    iso.SetField 11, strTraceNo            ' TraceNo
    iso.SetField 24, strNII                ' NII
    iso.SetField 39, "95"                  ' ResponseCode -> Bad settlement
    iso.SetField 41, strTerminalID         ' TerminalID
    
ElseIf (strMsgType = "0500" And Mid(strProcessingCode, 1, 2) = "96" And Mid(strProcessingCode, 4, 2) = "00")Then
    ColoredReport "Settlement Trailer", Violet
    strTraceNo = iso.GetField(11)
    If (strTraceNo = "") Then ColoredReport "System Trace Number" + MsgErr, Red
    strNII = iso.GetField(24)
    If (strNII = "") Then ColoredReport "NII" + MsgErr, Red
    strTerminalID = iso.GetField(41)
    If (strTerminalID = "") Then ColoredReport "Terminal ID" + MsgErr, Red
    If (iso.GetField(42) = "") Then ColoredReport "Acquirer ID" + MsgErr, Red

    iso.SetField 0, "0510"                 ' MsgType
    iso.SetField 3, "960000"               ' ProcessingCode
    iso.SetField 11, strTraceNo            ' TraceNo
    iso.SetField 24, strNII                ' NII
    iso.SetField 37, "000000000012"        ' RetRefNo
    iso.SetField 39, "00"                  ' ResponseCode
    iso.SetField 41, strTerminalID         ' TerminalID
    
ElseIf (strMsgType = "0320" And Mid(strProcessingCode, 1, 2) <> "90")Then
    ColoredReport "Batch Upload", Violet
    If (iso.GetField(22) = "") Then ColoredReport "POSEntryMode" + MsgErr, Red
    If (iso.GetField(25) = "") Then ColoredReport "POSConditionCode" + MsgErr, Red
    If (iso.GetField(37) = "") Then ColoredReport "retrieval RefNo" + MsgErr, Red
    
    Call Message
    iso.SetField 0, "0330"                 ' MsgType
    iso.SetField 3, "030000"               ' ProcessingCode

ElseIf (strMsgType = "0800" And Mid(strProcessingCode, 1, 2) = "91" And Mid(strProcessingCode, 4, 2) = "00")Then
    ColoredReport "Statistics", Violet
    strTraceNo = iso.GetField(11)
    If (strTraceNo = "") Then ColoredReport "System Trace Number" + MsgErr, Red
    strNII = iso.GetField(24)
    If (strNII = "") Then ColoredReport "NII" + Error, Red
    strTerminalID = iso.GetField(41)
    If (strTerminalID = "") Then ColoredReport "Terminal ID" + MsgErr, Red
        
    iso.SetField 0, "0810"                 ' MsgType
    iso.SetField 3, "910000"               ' ProcessingCode
    iso.SetField 11, strTraceNo            ' TraceNo
    iso.SetField 24, strNII                ' NII
    iso.SetField 39, "00"                  ' ResponseCode
    iso.SetField 41, strTerminalID         ' TerminalID
    iso.SetField 63, "010302072100000208707700717FFFFFFFFFFFFF" ' ScheduleCommand
    
Else
    ColoredReport "Unknown message type", Red

    intMsgType = CInt(strMsgType)
    iso.SetField 0, CStr(intMsgType+10)    ' MsgType
    iso.SetField 3, strProcessingCode      ' ProcessingCode
    If (iso.GetField(4) <> "") Then iso.SetField 4, iso.GetField(4)    ' TxnAmount
    If (iso.GetField(11) <> "") Then iso.SetField 11, iso.GetField(11) ' STAN
    If (iso.GetField(24) <> "") Then iso.SetField 24, iso.GetField(24) ' NII
    iso.SetField 37, "000000000012"        ' RetRefNo
    iso.SetField 39, "00"                  ' ResponseCode
    If (iso.GetField(41) <> "") Then iso.SetField 41, iso.GetField(41) ' TerminalID
End If

End Sub

'''''''''''''''''''''''''''' Message '''''''''''''''''''''''''''''''''''''''
Sub Message
MsgErr = " not present. ERROR"
    ' Check mandatory fields
    If (iso.GetField(4) = "") Then ColoredReport "Amount not specified", Red
    strTraceNo = iso.GetField(11)
    If (strTraceNo = "") Then ColoredReport "System Trace Number" + MsgErr, Red
    strNII = iso.GetField(24)
    If (strNII = "") Then ColoredReport "NII" + MsgErr, Red
    strTerminalID = iso.GetField(41)
    If (strTerminalID = "") Then ColoredReport "Terminal ID" + MsgErr, Red
    If (iso.GetField(42) = "") Then ColoredReport "Acquirer ID" + MsgErr, Red
        
    ' Form the response
    iso.SetField 11, strTraceNo            ' TraceNo
    iso.SetField 24, strNII                ' NII
    iso.SetField 37, "000000000012"        ' RetRefNo
    iso.SetField 38, "123ABC"              ' AutnID
    iso.SetField 39, "00"                  ' ResponseCode
    iso.SetField 41, strTerminalID         ' TerminalID
End Sub

'''''''''''''''''''''''''' ColoredReport '''''''''''''''''''''''''''''''''''
Function ColoredReport (String, Color)
    Name = "STIM"
'   MsgBox string, 4096, Name
    txtWnd.ColoredWrite String, Color
End Function

'''''''''''''''''''''''''' SetColors '''''''''''''''''''''''''''''''''''
Function SetColors
    Red     = 1
    Blue    = 2
    Green   = 3
    Violet  = 4
    Orange  = 5
    Grey    = 6
End Function

'''''''''''''''''''''''''' CheckField55 '''''''''''''''''''''''''''''''''''
Function CheckField55
    If (iso.GetField(55) <> "") Then
    	If(iso.GetTagFld55("5F2A") = "") Then ColoredReport "Mandatory tag 5F2A is not present. ERROR", Red
    	If(iso.GetTagFld55("82") = "") Then ColoredReport "Mandatory tag 82 is not present. ERROR", Red
    	If(iso.GetTagFld55("95") = "") Then ColoredReport "Mandatory tag 95 is not present. ERROR", Red
    	If(iso.GetTagFld55("9A") = "") Then ColoredReport "Mandatory tag 9A is not present. ERROR", Red
    	If(iso.GetTagFld55("9C") = "") Then ColoredReport "Mandatory tag 9C is not present. ERROR", Red
    	If(iso.GetTagFld55("9F02") = "") Then ColoredReport "Mandatory tag 9F02 is not present. ERROR", Red
    	If(iso.GetTagFld55("9F03") = "") Then ColoredReport "Mandatory tag 9F03 is not present. ERROR", Red
    	If(iso.GetTagFld55("9F10") = "") Then ColoredReport "Mandatory tag 9F10 is not present. ERROR", Red
    	If(iso.GetTagFld55("9F1A") = "") Then ColoredReport "Mandatory tag 9F1A is not present. ERROR", Red
    	If(iso.GetTagFld55("9F26") = "") Then ColoredReport "Mandatory tag 9F26 is not present. ERROR", Red
    	If(iso.GetTagFld55("9F27") = "") Then ColoredReport "Mandatory tag 9F27 is not present. ERROR", Red
    	If(iso.GetTagFld55("9F36") = "") Then ColoredReport "Mandatory tag 9F36 is not present. ERROR", Red
    	If(iso.GetTagFld55("9F37") = "") Then ColoredReport "Mandatory tag 9F37 is not present. ERROR", Red
    End If
End Function
