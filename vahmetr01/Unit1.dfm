object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'E-154 L-CARD'
  ClientHeight = 618
  ClientWidth = 795
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 8
    Top = 8
    Width = 779
    Height = 601
    Caption = 'E - 154'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object Label3: TLabel
      Left = 16
      Top = 106
      Width = 35
      Height = 14
      Caption = 'Label3'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Button1: TButton
      Left = 16
      Top = 28
      Width = 121
      Height = 43
      Caption = #1057#1090#1072#1088#1090
      TabOrder = 0
      OnClick = Button1Click
    end
    object Button2: TButton
      Left = 512
      Top = 95
      Width = 137
      Height = 41
      Caption = #1052#1091#1083#1100#1090#1080#1084#1077#1090#1088
      TabOrder = 1
    end
    object Button3: TButton
      Left = 160
      Top = 28
      Width = 113
      Height = 45
      Caption = #1057#1090#1086#1087
      TabOrder = 2
      OnClick = Button3Click
    end
    object CheckBox1: TCheckBox
      Left = 16
      Top = 83
      Width = 297
      Height = 17
      Caption = #1054#1090#1089#1091#1090#1074#1080#1077' '#1086#1090#1088#1080#1094#1072#1090#1077#1083#1100#1085#1086#1081' '#1074#1077#1090#1074#1080
      TabOrder = 3
    end
    object GroupBox2: TGroupBox
      Left = 319
      Top = 16
      Width = 378
      Height = 73
      Caption = #1052#1072#1082#1089#1080#1084#1072#1083#1100#1085#1086#1077' '#1079#1085#1072#1095#1077#1085#1080#1077' '#1085#1072#1087#1088#1103#1078#1077#1085#1080#1103
      TabOrder = 4
      object Button4: TButton
        Left = 16
        Top = 32
        Width = 75
        Height = 25
        Caption = '0.2 '#1042
        TabOrder = 0
        OnClick = Button4Click
      end
      object Button5: TButton
        Left = 112
        Top = 32
        Width = 75
        Height = 25
        Caption = '0.5 '#1042
        TabOrder = 1
        OnClick = Button5Click
      end
      object Button6: TButton
        Left = 209
        Top = 32
        Width = 75
        Height = 25
        Caption = '1 '#1042
        TabOrder = 2
        OnClick = Button6Click
      end
      object Button7: TButton
        Left = 297
        Top = 32
        Width = 75
        Height = 25
        Caption = '2 '#1042
        TabOrder = 3
        OnClick = Button7Click
      end
    end
    object GroupBox3: TGroupBox
      Left = 512
      Top = 303
      Width = 264
      Height = 274
      Caption = #1054#1082#1085#1086' '#1087#1086#1076#1089#1082#1072#1079#1086#1082
      TabOrder = 5
      object Memo1: TMemo
        Left = 16
        Top = 29
        Width = 233
        Height = 140
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ScrollBars = ssVertical
        TabOrder = 0
      end
      object Button8: TButton
        Left = 14
        Top = 239
        Width = 99
        Height = 25
        Caption = #1054#1095#1080#1089#1090#1080#1090#1100
        TabOrder = 1
      end
    end
    object GroupBox4: TGroupBox
      Left = 515
      Top = 142
      Width = 246
      Height = 66
      Caption = #1058#1054#1050' ('#1084#1082#1040')'
      TabOrder = 6
      object Label1: TLabel
        Left = 14
        Top = 32
        Width = 69
        Height = 25
        Caption = 'Label1'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -21
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
    end
    object GroupBox5: TGroupBox
      Left = 515
      Top = 223
      Width = 246
      Height = 74
      Caption = #1053#1040#1055#1056#1071#1046#1045#1053#1048#1045' ('#1084#1042')'
      TabOrder = 7
      object Label2: TLabel
        Left = 14
        Top = 32
        Width = 69
        Height = 25
        Caption = 'Label2'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -21
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
    end
    object Chart1: TChart
      Left = 3
      Top = 122
      Width = 503
      Height = 455
      Legend.Visible = False
      Title.Text.Strings = (
        #1042#1086#1083#1100#1090'-'#1072#1084#1087#1077#1088#1085#1072#1103' '#1093#1072#1088#1072#1082#1090#1077#1088#1080#1089#1090#1080#1082#1072)
      BottomAxis.Automatic = False
      BottomAxis.AutomaticMaximum = False
      BottomAxis.AutomaticMinimum = False
      BottomAxis.Maximum = 24.000000000000000000
      BottomAxis.Title.Caption = #1053#1072#1087#1088#1103#1078#1077#1085#1080#1077', '#1042
      LeftAxis.Automatic = False
      LeftAxis.AutomaticMaximum = False
      LeftAxis.AutomaticMinimum = False
      LeftAxis.Maximum = 990.000000000000000000
      LeftAxis.Minimum = 275.000000000000000000
      LeftAxis.Title.Caption = #1058#1086#1082', '#1084#1040
      RightAxis.Automatic = False
      RightAxis.AutomaticMaximum = False
      RightAxis.AutomaticMinimum = False
      TopAxis.Automatic = False
      TopAxis.AutomaticMaximum = False
      TopAxis.AutomaticMinimum = False
      TopAxis.Title.Caption = #1042#1086#1083#1100#1090'-'#1072#1084#1087#1077#1088#1085#1072#1103' '#1093#1072#1088#1072#1082#1090#1077#1088#1080#1089#1090#1080#1082#1072
      TabOrder = 8
      ColorPaletteIndex = 13
      object Series1: TFastLineSeries
        Marks.Arrow.Visible = True
        Marks.Callout.Brush.Color = clBlack
        Marks.Callout.Arrow.Visible = True
        Marks.Visible = False
        SeriesColor = clRed
        LinePen.Color = clRed
        LinePen.Width = 4
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Y'
        YValues.Order = loNone
      end
    end
  end
  object MainMenu1: TMainMenu
    Left = 696
    Top = 16
    object N1: TMenuItem
      Caption = #1058#1077#1093'. '#1055#1086#1076#1076#1077#1088#1078#1082#1072
      object N4: TMenuItem
        Caption = #1054#1090#1082#1088#1099#1090#1100
      end
    end
    object N2: TMenuItem
      Caption = #1054' '#1087#1088#1086#1075#1088#1072#1084#1084#1077
      object N5: TMenuItem
        Caption = #1054#1090#1082#1088#1099#1090#1100
      end
    end
    object N3: TMenuItem
      Caption = #1048#1085#1089#1090#1088#1091#1082#1094#1080#1103
      object N6: TMenuItem
        Caption = #1054#1090#1082#1088#1099#1090#1100
      end
    end
  end
  object Timer1: TTimer
    OnTimer = Timer1Timer
    Left = 680
    Top = 256
  end
end
