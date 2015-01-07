// Animation

module animation
    (    LEDG,
        LEDR,
        SW,
        CLOCK_50,                        //    On Board 50 MHz
        KEY,                            //    Push Button[3:0]
        VGA_CLK,                           //    VGA Clock
        VGA_HS,                            //    VGA H_SYNC
        VGA_VS,                            //    VGA V_SYNC
        VGA_BLANK,                        //    VGA BLANK
        VGA_SYNC,                        //    VGA SYNC
        VGA_R,                           //    VGA Red[9:0]
        VGA_G,                             //    VGA Green[9:0]
        VGA_B                           //    VGA Blue[9:0]
    );

    input [17:0] SW;
    input            CLOCK_50;                //    50 MHz
    input    [3:0]    KEY;                    //    Button[3:0]
    output            VGA_CLK;                   //    VGA Clock
    output            VGA_HS;                    //    VGA H_SYNC
    output            VGA_VS;                    //    VGA V_SYNC
    output            VGA_BLANK;                //    VGA BLANK
    output            VGA_SYNC;                //    VGA SYNC
    output    [9:0]    VGA_R;                   //    VGA Red[9:0]
    output    [9:0]    VGA_G;                     //    VGA Green[9:0]
    output    [9:0]    VGA_B;                   //    VGA Blue[9:0]
    output [17:0] LEDR;
    output [3:0] LEDG;
 
    wire resetn;
     assign resetn = ~SW[16];
 
    // Create the color, x, y and writeEn wires that are inputs to the controller.

    wire [2:0] color;
    wire [7:0] x;
    wire [6:0] y;
    wire writeEn, diving, jumping, SECONDdiving, SECONDjumping;
 
    assign LEDR[0] = writeEn;
    assign LEDG[1] = ~KEY[1];
    wire Draw, Erase;
 
     assign LEDR[17] = Draw;
     assign LEDR[16] = Erase;
 
      wire [7:0] XIN, SECONDXIN;
      wire [6:0]YIN, SECONDYIN;
 
      JUMP j1(CLOCK_50, SW[14],~KEY[1],~KEY[0],XIN, YIN, diving, jumping);
      SECONDJUMP j2(CLOCK_50, SW[14],~KEY[3],~KEY[2],SECONDXIN, SECONDYIN, SECONDdiving, SECONDjumping);
 // JUMP(input Clock, BeginEN, DiveEN, JumpEN, output reg[7:0] XIN, output reg[6:0] YIN);
     FSMdrawANDerase s1(CLOCK_50, SW[17], Draw, Erase);
                                            //Reset
                                                       
        wire COLLISION, winner1, winner2;
        assign LEDR[10] = COLLISION;

          assign LEDR[7] = winner1;
          assign LEDR[6] = winner2;
             
             wire playerOneDistance, playerTwoDistance;
         
             assign winner1 = (COLLISION&&diving&&jumping&&(YIN<SECONDYIN))||(TimeWin1);
             assign winner2 = (COLLISION&&SECONDdiving&&SECONDjumping&&(YIN>SECONDYIN))||(TimeWin2);
         //SEVEN IS WHEN DIVE WINS
         //SIX IS WHEN RONALD WINS
           
            //LEAVE 15 ON, FLICK 13, FLICK 14, FLICK 13
        
        COLLISIONDETECTED a1(SW[14], XIN, YIN, SECONDXIN, SECONDYIN, COLLISION);
         
          wire TimesUp;
          assign TimesUp = (Timer3&&Timer2&&Timer1);
          wire TimeWin1, TimeWin2;
         
          checkCloser cc1(TimesUp, XIN, SECONDXIN, TimeWin1, TimeWin2);

      dataPath u1(Timer3, Timer2, Timer1, SW[12], SW[13], winner1, winner2, diving, jumping, XIN, YIN, SECONDdiving, SECONDjumping, SECONDXIN, SECONDYIN, Draw, CLOCK_50, SW[15], Erase, x, y, color, writeEn);
//dataPath(input[7:0] Xin, input [6:0] Yin, input start, Clock, Resetn, Clear, output reg [7:0] X_out, output reg [6:0] Y_out, output reg [2:0] color, output reg EnableOut = 0);
       
        wire Timer3, Timer2, Timer1;
       
        assign LEDR[3] = Timer3;
        assign LEDR[2] = Timer2;
        assign LEDR[1] = Timer1;
       
        TimerClock Time1(SW[14], CLOCK_50, Timer3, Timer2, Timer1);

    // Create an Instance of a VGA controller - there can be only one!
    // Define the number of colours as well as the initial background
    // image file (.MIF) for the controller.
    vga_adapter VGA(
            .resetn(resetn),
            .clock(CLOCK_50),
            .colour(color),
            .x(x),
            .y(y),
            .plot(writeEn),
            /* Signals for the DAC to drive the monitor. */
            .VGA_R(VGA_R),
            .VGA_G(VGA_G),
            .VGA_B(VGA_B),
            .VGA_HS(VGA_HS),
            .VGA_VS(VGA_VS),
            .VGA_BLANK(VGA_BLANK),
            .VGA_SYNC(VGA_SYNC),
            .VGA_CLK(VGA_CLK));
        defparam VGA.RESOLUTION = "160x120";
        defparam VGA.MONOCHROME = "FALSE";
        defparam VGA.BITS_PER_COLOUR_CHANNEL = 1;
        defparam VGA.BACKGROUND_IMAGE = "DIVEKICK.mif";
     
    // Put your code here. Your code should produce signals x,y,color and writeEn
    // for the VGA controller, in addition to any other functionality your design may require.
 
 
endmodule

module checkCloser(input Clock, input [7:0] Xin, input [7:0]SECONDXin, output reg Winner1 = 0, Winner2 = 0);
    reg [7:0] Xdistance, SecondXdistance;
   
    always@(*) begin
   
            if (Xin > 79)
                Xdistance = Xin - 80;
            else
                Xdistance = 80 - Xin;
               
            if (SECONDXin > 79)
                SecondXdistance = SECONDXin - 80;
            else
                SecondXdistance = 80 - SECONDXin;
           
            if (Clock)
                if(SecondXdistance>Xdistance) begin
                    Winner1 = 1;
                    Winner2 = 0;
                end
                else begin
                    Winner1 = 0;
                    Winner2 = 1;
                end
            else begin
                Winner1 = 0;
                Winner2 = 0;
            end
           
    end
endmodule

module dataPath(input Three, Two, One, HitBox, MasterReset, Winner1, Winner2, DIVING, JUMPING, input[7:0] Xin, input [6:0] Yin, input SECONDDIVING, SECONDJUMPING, input[7:0] SECONDXin, input [6:0] SECONDYin, input start, Clock, Resetn, Clear, output reg [7:0] X_out, output reg [6:0] Y_out, output reg [2:0] color, output reg EnableOut = 0);
 
     reg[10:0] address = 0;
      reg[14:0] addressh = 0;
        reg[14:0] addressWin = 0;
    wire Clock_60Hz;
    reg[7:0] X_adder = 0;
    reg[6:0] Y_adder = 0;
     wire [2:0] colorDive, colorStand, colorJump, colorBG, colorBG1, colorBG2, colorBG3, colorBG4, colorBG5, colorDiveWin, colorRonWin;
     wire [2:0] SecondDive, SecondStand, SecondJump;
     reg Enabling = 0;
     reg Clearing = 0;
     reg FIRST = 0;
     reg SECOND = 0;
     
      reg DiveWin = 0;
      reg RonWin = 0;
      reg Resetting = 0;
    
     characterDive d1(address, Clock, colorDive);
      characterJump j1(address, Clock, colorJump);
     characterstand s1(address, Clock, colorStand);
 
     RonaldDive d2(address, Clock, SecondDive);
     RonaldJump j2(address, Clock, SecondJump);
     RonaldStand s2(address, Clock, SecondStand);

     Heaven heav1(addressh, Clock, colorBG);
      Haeven1 h1(addressh, Clock, colorBG1);
      Haeven2 h2(addressh, Clock, colorBG2);
      Haeven3 h3(addressh, Clock, colorBG3);
      Haeven4 h4(addressh, Clock, colorBG4);
      Haeven5 h5(addressh, Clock, colorBG5);
     
      DiveWinner dw1(addressWin, Clock, colorDiveWin);
      RonaldWinner r1(addressWin, Clock, colorRonWin);
     
 
    always @ (posedge Clock) begin
 
           
                if (Winner1) begin
                    Enabling = 0;
                    Clearing = 0;
                    EnableOut = 1;
                    DiveWin = 1;
                end
                else if (Winner2) begin
                    Enabling = 0;
                    Clearing = 0;
                    EnableOut = 1;
                    RonWin = 1;
                end
                else if (MasterReset) begin
                    Enabling = 0;
                    EnableOut = 0;
                    Clearing = 0;
                    Resetting = 1;
                end
                else if(start) begin
                Enabling = 1;
                EnableOut = 1;
                     FIRST = 1;
            end
            else if(Clear) begin
                Clearing = 1;
                EnableOut = 1;
            end
     
                if(DiveWin) begin
                    if(Resetting) begin
                        DiveWin = 0;
                        EnableOut = 0;
                        Resetting = 0;
                    end
                    else begin
                     addressWin = (X_adder + 160*Y_adder);

                     color = colorDiveWin;
             
                     X_out = X_adder;
                     Y_out = Y_adder;
           
                    if (X_adder < 160) begin
                              X_adder = X_adder + 1;
                         end
           
                    if (X_adder == 160 & Y_adder < 120) begin
                              X_adder = 0;
                              Y_adder = Y_adder + 1;
                         end
                 
                    if (Y_adder == 120) begin
                              X_adder = 0;
                              Y_adder = 0;
                         end
                     end
            end
                   
                else if(RonWin) begin
                    if (Resetting) begin
                        RonWin = 0;
                        EnableOut = 0;
                        Resetting = 0;
                    end
                    else begin
                     addressWin = (X_adder + 160*Y_adder);

                     color = colorRonWin;
             
                     X_out = X_adder;
                     Y_out = Y_adder;
           
                    if (X_adder < 160) begin
                              X_adder = X_adder + 1;
                         end
           
                    if (X_adder == 160 & Y_adder < 120) begin
                              X_adder = 0;
                              Y_adder = Y_adder + 1;
                         end
                 
                    if (Y_adder == 120) begin
                              X_adder = 0;
                              Y_adder = 0;
                         end
                     end
            end
               
           else if (Enabling) begin
        if (FIRST) begin
          address = (X_adder + 30*Y_adder);

          X_out = Xin + X_adder;
          Y_out = Yin + Y_adder;
          
              if(DIVING&&JUMPING)
              color = colorDive;
              else if(DIVING||JUMPING)
              color = colorJump;
              else if(~DIVING&&~JUMPING)
              color = colorStand;
                 
             if ((color == 3'b111)&&(~HitBox))
                EnableOut = 0;
            else
                EnableOut = 1;
 
          if (X_adder < 30) begin
              X_adder = X_adder + 1;
              end
      
          if (X_adder == 30 & Y_adder < 40) begin
            X_adder = 0;
              Y_adder = Y_adder + 1;
              end
        
          if (Y_adder == 40) begin
              X_adder = 0;
              Y_adder = 0;
              FIRST = 0;
              SECOND = 1;
          end
          end
          else if(SECOND)begin
          address = (X_adder + 30*Y_adder);

          X_out = SECONDXin + X_adder;
          Y_out = SECONDYin + Y_adder;
          
              if(SECONDDIVING&&SECONDJUMPING)
              color = SecondDive;
              else if(SECONDDIVING||SECONDJUMPING)
              color = SecondJump;
              else if(~SECONDDIVING&&~SECONDJUMPING)
              color = SecondStand;
                 
             if ((color == 3'b111)&&(~HitBox))
                EnableOut = 0;
             else
                EnableOut = 1;
  
          if (X_adder < 30) begin
              X_adder = X_adder + 1;
              end
      
          if (X_adder == 30 & Y_adder < 40) begin
            X_adder = 0;
              Y_adder = Y_adder + 1;
              end
        
          if (Y_adder == 40) begin
              X_adder = 0;
              Y_adder = 0;
              SECOND = 0;
              EnableOut = 0;
              Enabling = 0;
          end
          end
       end
     
            else if (Clearing) begin
         
                addressh = (X_adder + 160*Y_adder);
                   
                    if (addressh < 1761) begin
                        if (Three&&Two&&~One)
                            color = colorBG5;
                        else if (Three&&~Two&&One)
                            color = colorBG4;
                        else if (Three&&~Two&&~One)
                            color = colorBG3;
                        else if (~Three&&Two&&One)
                            color = colorBG2;
                        else if (~Three&&Two&&~One)
                            color = colorBG1;
                        else if (~Three&&~Two)
                            color = colorBG;
                    end
                    else
                        color = colorBG;
                     
         
                X_out = X_adder;
                Y_out = Y_adder;
     
            if (X_adder < 160) begin
                    X_adder = X_adder + 1;
                end
     
            if (X_adder == 160 & Y_adder < 120) begin
                    X_adder = 0;
                    Y_adder = Y_adder + 1;
                end
         
            if (Y_adder == 120) begin
                    X_adder = 0;
                    Y_adder = 0;
                    EnableOut = 0;
                    Clearing = 0;
                end
            end
         
 
            else if (Resetn) begin
                X_adder = 0;
                Y_adder = 0;
                EnableOut = 0;
            end
end
endmodule
 
module TwoSecClock(input Clock, output reg EN = 0);
    reg[30:0] cnt = 0;
    always @ (posedge Clock) begin
        cnt = cnt + 1;
        if (cnt == 200060) begin
            cnt = 0;
            EN = ~EN;
        end
    end
endmodule

module OneSecClock(input Clock, output reg EN = 0);
    reg[30:0] cnt = 0;
    always @ (posedge Clock) begin
        cnt = cnt + 1;
        if (cnt ==100030) begin
            cnt = 0;
            EN = ~EN;
        end
    end
endmodule

module HalfSecClock(input Clock, output reg EN = 0);
    reg[30:0] cnt = 0;
    always @ (posedge Clock) begin
        cnt = cnt + 1;
        if (cnt ==50015) begin
            cnt = 0;
            EN = ~EN;
        end
    end
endmodule

module TimerClock(input Reset, Clock, output reg Three = 0, Two = 0, One = 0);
    reg[30:0] cnt = 0;

    always @ (posedge Clock) begin
     
            if (cnt < 100000000)
                cnt = cnt + 1;
        else if (cnt == 100000000) begin
         
                if(Three&&Two&&One&&~Reset) begin
                    Three = 1;
                    Two = 1;
                    One = 1;
                end
                else if (Reset) begin
                    Three = 0;
                    Two = 0;
                    One = 0;
                end
                else begin
                    if ((Two == 1)&&(One == 1))
                        Three = ~Three;
                       
                    if (One == 1)
                        Two = ~Two;
                       
                    One = ~One;
                    cnt = 0;
                end
        end
               
    end
endmodule


module FSMdrawANDerase(input Clock, Enable, output reg Draw = 0, Erase = 0);

    wire q,r,s;
    TwoSecClock q1(Clock, q);
    OneSecClock r1(Clock, r);
     HalfSecClock s1(Clock, s);
 
always@(*)begin
    if (Enable)begin
     Draw = ((q == 1 && r == 1)||(q == 0 && r == 1));
     Erase = (q == 1 && r == 0 && s == 0);
    end
    else begin
        Draw = 0;
        Erase = 0;
    end
end
endmodule

module JUMP(input Clock, BeginEN, DiveEN, JumpEN, output reg[7:0] XIN, output reg[6:0] YIN, output JUMPING, DIVING);

    wire Clock_60;
    _60HzClock s1(Clock, Clock_60);
 
    reg [7:0]X_add;
    reg [6:0]Y_add;
    reg [10:0]Count;
    reg    BEGIN, DIVE, JumpUpwards,JumpDownwards,JUMP;
 
    initial begin
         XIN <= 10;
         YIN <= 75;
         X_add <= 0;
         Y_add <= 0;
         JumpUpwards <= 0;
         JumpDownwards <= 0;
         JUMP <= 0;
         DIVE <= 0;
         Count<=0;
    end
 
    always@(posedge Clock_60)begin
 
        if(BeginEN)begin
            BEGIN = 1;
        end
 
        else if(BEGIN)begin
             XIN <= 10;
             YIN <= 75;
             X_add <= 0;
             Y_add <= 0;
             JumpUpwards <= 0;
             JumpDownwards <= 0;
             JUMP <= 0;
             BEGIN <= 0;
         end
   
        else if (JumpEN)begin
             YIN = 75;
             Y_add = 0;
             JumpUpwards <= 0;
             JumpDownwards <= 0;
             JUMP <=1;
      end
       
        else if (DiveEN) begin
             DIVE <= 1;
             X_add <= 1;
             Y_add <= 1;
             Count <= 0;
        end
   
        else if(DIVE&~JUMP)begin
            if(Count<55)begin
                Y_add = 0;
                X_add = -1;
                Count = Count + 1;
            end
       
            if (Count == 55) begin
                X_add = 0;
                Y_add =0;
                Count = 0;
                DIVE = 0;
            end
       
            YIN = YIN + Y_add;
            XIN = XIN + X_add;
       
        end
   
            else if(JUMP)begin
                if(DIVE)begin
                    if(YIN==75)begin
                         X_add = 0;
                         Y_add = 0;
                         JumpUpwards = 0;
                         JumpDownwards = 0;
                         JUMP = 0;
                         DIVE = 0;
                     end
                     else begin
                        X_add = 1;
                        Y_add = 1;
                    end
                end
           
                else begin
                    if (~JumpUpwards&~JumpDownwards)begin
                         JumpUpwards = 1;
                    end
                    else if (YIN>1&JumpUpwards)begin
                         Y_add = - 1;
                    end
                    else if (YIN==1&JumpUpwards)begin
                         JumpDownwards = 1;
                         JumpUpwards = 0;
                    end
                    else if (YIN<75&JumpDownwards)begin
                         Y_add = 1;
                    end
                    else if (YIN==75&JumpDownwards)begin
                         Y_add = 0;
                         JumpDownwards = 0;
                         JUMP = 0;
                    end
                end
                     XIN = XIN + X_add;
                     YIN = YIN + Y_add;
            end
    end

assign DIVING = DIVE;
assign JUMPING = JUMP;
endmodule

module _60HzClock(input Clock, output reg EN = 0);
    reg[19:0] cnt = 0;
    always @ (posedge Clock) begin
        cnt = cnt + 1;
          EN = 0;
        if (cnt == 300000) begin
            cnt = 0;
            EN = 1;
        end
    end
endmodule


module SECONDJUMP(input Clock, BeginEN, DiveEN, JumpEN, output reg[7:0] XIN, output reg[6:0] YIN, output JUMPING, DIVING);

    wire Clock_60;
    _60HzClock sh1(Clock, Clock_60);
 
    reg [7:0]X_add;
    reg [6:0]Y_add;
    reg [10:0]Count;
    reg    BEGIN, DIVE, JumpUpwards,JumpDownwards,JUMP;
 
    initial begin
         XIN <= 120;
         YIN <= 75;
         X_add <= 0;
         Y_add <= 0;
         JumpUpwards <= 0;
         JumpDownwards <= 0;
         JUMP <= 0;
         DIVE <= 0;
         Count<=0;
    end
 
    always@(posedge Clock_60)begin
 
        if(BeginEN)begin
            BEGIN = 1;
        end
 
        else if(BEGIN)begin
             XIN <= 120;
             YIN <= 75;
             X_add <= 0;
             Y_add <= 0;
             JumpUpwards <= 0;
             JumpDownwards <= 0;
             JUMP <= 0;
             BEGIN <= 0;
         end
   
        else if (JumpEN)begin
             YIN = 75;
             Y_add = 0;
             JumpUpwards <= 0;
             JumpDownwards <= 0;
             JUMP <=1;
      end
       
        else if (DiveEN) begin
             DIVE <= 1;
             X_add <= -1;
             Y_add <= 1;
             Count <= 0;
        end
   
        else if(DIVE&~JUMP)begin
            if(Count<55)begin
                Y_add = 0;
                X_add = 1;
                Count = Count + 1;
            end
       
            if (Count == 55) begin
                X_add = 0;
                Y_add =0;
                Count = 0;
                DIVE = 0;
            end
       
            YIN = YIN + Y_add;
            XIN = XIN + X_add;
       
        end
   
            else if(JUMP)begin
                if(DIVE)begin
                    if(YIN==75)begin
                         X_add = 0;
                         Y_add = 0;
                         JumpUpwards = 0;
                         JumpDownwards = 0;
                         JUMP = 0;
                         DIVE = 0;
                     end
                     else begin
                        X_add = -1;
                        Y_add = 1;
                    end
                end
           
                else begin
                    if (~JumpUpwards&~JumpDownwards)begin
                         JumpUpwards = 1;
                    end
                    else if (YIN>1&JumpUpwards)begin
                         Y_add = - 1;
                    end
                    else if (YIN==1&JumpUpwards)begin
                         JumpDownwards = 1;
                         JumpUpwards = 0;
                    end
                    else if (YIN<75&JumpDownwards)begin
                         Y_add = 1;
                    end
                    else if (YIN==75&JumpDownwards)begin
                         Y_add = 0;
                         JumpDownwards = 0;
                         JUMP = 0;
                    end
                end
                     XIN = XIN + X_add;
                     YIN = YIN + Y_add;
            end
    end

assign DIVING = DIVE;
assign JUMPING = JUMP;
endmodule

module COLLISIONDETECTED (input reset,input [7:0]XIN, input [6:0]YIN, input [7:0] SECONDXIN, input [6:0] SECONDYIN, output reg Collision = 0);
 
    always@(*)begin
      if(reset) begin
            Collision = 0;
      end
      else begin
          if((((XIN + 20) >= SECONDXIN)&&(XIN <= SECONDXIN))&&((YIN + 35) >= SECONDYIN)&&(YIN <= (SECONDYIN + 35)))
                Collision = 1;
          else
                Collision = 0;
      end
    end

endmodule
