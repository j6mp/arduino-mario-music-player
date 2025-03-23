
int buzzer = 8;
int led1 = 13;  // 內建LED
int led2 = 12;  // 新增第二個LED
int led3 = 11;  // 新增第三個LED
int button = 2; // 按鈕腳位
int gndPin = 4; // 改用腳位4作為GND
int playState = 1; // 1表示播放，0表示暫停
int lastButtonState = HIGH; // 上一次按鈕狀態
int currentButtonState = HIGH; // 當前按鈕狀態
int buttonPressed = false; // 新增按鈕狀態標記

//音階頻率 (增加高音階以支援瑪利歐音樂)
int freq[] = {262, 294, 330, 349, 392, 440, 494, 523, 587, 659, 698, 784, 880, 988, 1047}; // 增加高音階
//瑪利歐主題曲旋律
int melody[] = {
  10,10,0,10,0,7,10,0,12,0,0,0,11,10,9,8,9,0,0,0,
  10,10,0,10,0,7,10,0,12,0,0,0,11,10,9,8,9,0,0,0,
  10,10,0,10,0,7,10,0,12,0,0,0,11,10,9,8,9,0,0,0,
  10,10,0,10,0,7,10,0,12,0,0,0,11,10,9,8,9,0,0,0
};
//瑪利歐主題曲節拍
int beat[] = {
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};

void setup() {
  pinMode(buzzer, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(button, INPUT_PULLUP); // 設定按鈕為上拉輸入
  pinMode(gndPin, OUTPUT); // 設定GND腳位為輸出
  digitalWrite(gndPin, LOW); // 將GND腳位設為低電位
}

void loop() {
  // 讀取按鈕狀態
  currentButtonState = digitalRead(button);
  
  // 檢測按鈕按下
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    // 按鈕被按下
    buttonPressed = true;
  }
  
  // 檢測按鈕釋放
  if (lastButtonState == LOW && currentButtonState == HIGH && buttonPressed) {
    // 按鈕被釋放，切換播放狀態
    playState = !playState;
    buttonPressed = false;
    
    if (!playState) {
      noTone(buzzer); // 暫停時停止蜂鳴器
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
    }
  }
  
  // 更新按鈕狀態
  lastButtonState = currentButtonState;
  
  // 只在播放狀態下執行音樂播放
  if (playState) {
    for(int i = 0; i < 80; i++) {
      // 檢查按鈕是否被按下
      if (digitalRead(button) == LOW) {
        playState = 0;
        noTone(buzzer);
        digitalWrite(led1, LOW);
        digitalWrite(led2, LOW);
        digitalWrite(led3, LOW);
        break;
      }
      
      // 每個節拍都閃爍LED
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, HIGH);
      delay(75); // LED亮起時間加長
      
      if(melody[i] != 0) {
        // 根據音高控制不同的LED
        if(melody[i] > 10) {
          digitalWrite(led1, HIGH);
          digitalWrite(led2, LOW);
          digitalWrite(led3, LOW);
        } else if(melody[i] > 7) {
          digitalWrite(led1, LOW);
          digitalWrite(led2, HIGH);
          digitalWrite(led3, LOW);
        } else {
          digitalWrite(led1, LOW);
          digitalWrite(led2, LOW);
          digitalWrite(led3, HIGH);
        }
        
        // 播放音符
        tone(buzzer, freq[melody[i]-1]);
        delay(beat[i] * 200 - 75); // 調整為0.75倍速 (150/0.75 = 200)
        noTone(buzzer);
      } else {
        // 休止符時保持所有LED熄滅
        digitalWrite(led1, LOW);
        digitalWrite(led2, LOW);
        digitalWrite(led3, LOW);
        delay(beat[i] * 200 - 75); // 調整為0.75倍速
      }
      
      // 音符結束時熄滅LED
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
    }
    // 播放完一輪後短暫停頓
    delay(750); // 調整重複間隔
  }
}
