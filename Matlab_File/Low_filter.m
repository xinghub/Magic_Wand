clc;
clear;
close all;
% 参数配置
CH_COUNT = 6;               % 需要根据实际通道数修改
COM_PORT = "COM38";
BAUD_RATE = 921600;
FRAME_SIZE = CH_COUNT*4 + 4; % 每帧字节数
BUFFER_SIZE = 2000;          % 显示数据点数
FFT_POINTS = 1024;          % FFT分析点数

% 初始化串口
s = serialport(COM_PORT, BAUD_RATE);
configureTerminator(s, 0);
s.Timeout = 10;

% 数据缓冲区初始化
timeBuffer = nan(1, BUFFER_SIZE);
dataBuffer = nan(CH_COUNT, BUFFER_SIZE);
ptr = 1;

% 创建图形界面
figure('Name','实时数据分析','NumberTitle','off','Position',[100 100 1200 800]);

% 时域子图
for ch = 1:CH_COUNT
    subplot(CH_COUNT,2,ch*2-1);
    hTime(ch) = animatedline('Color',[0.2 0.5 0.9]);
    title(['通道 ',num2str(ch),' 时域波形']);
    xlabel('时间 (s)'); ylabel('幅值');
    grid on;
end

% 频域子图
for ch = 1:CH_COUNT
    subplot(CH_COUNT,2,ch*2);
    hFreq(ch) = plot(0,0,'r');
    title(['通道 ',num2str(ch),' 频谱']);
    xlabel('频率 (Hz)'); ylabel('幅值');
    grid on;
end

startTime = tic;

% 主循环
while ishandle(gcf)
    % 读取完整帧数据
    while s.NumBytesAvailable >= FRAME_SIZE
        % 读取并解析数据
        rawData = read(s, FRAME_SIZE, "uint8"); % 使用 fread 读取为 uint8 类型
        % 强制转换为 uint8 类型
        rawData = uint8(rawData);
        frameData = typecast(rawData(1:end-4), 'single');
        frameData = double(frameData);
        tail = rawData(end-3:end);
        
        % 校验数据尾部
        if isequal(tail, [0,0,128,127])
            % 更新数据缓冲区
            currentTime = toc(startTime);
            timeBuffer(ptr) = currentTime;
            dataBuffer(:,ptr) = frameData;
            
            % 更新时域显示
            for ch = 1:CH_COUNT
                addpoints(hTime(ch), currentTime, frameData(ch));
            end
            
            % 更新频域显示（当数据足够时）
            if ptr > FFT_POINTS
                validIdx = mod(ptr-FFT_POINTS:ptr-1, BUFFER_SIZE) + 1;
                timeWindow = timeBuffer(validIdx);
                Fs = 1/mean(diff(timeWindow));
                
                for ch = 1:CH_COUNT
                    % 计算FFT
                    signal = dataBuffer(ch, validIdx);
                    Y = fft(detrend(signal), FFT_POINTS);
                    P2 = abs(Y/FFT_POINTS);
                    P1 = P2(1:FFT_POINTS/2+1);
                    P1(2:end-1) = 2*P1(2:end-1);
                    f = Fs*(0:(FFT_POINTS/2))/FFT_POINTS;
                    
                    % 更新频域图
                    set(hFreq(ch), 'XData',f, 'YData',P1);
                    subplot(CH_COUNT,2,ch*2);
                    xlim([0 Fs/2]);
                end
            end
            
            % 更新缓冲区指针
            ptr = mod(ptr, BUFFER_SIZE) + 1;
        else
            warning('无效数据帧：尾部校验失败');
        end
    end
    
    % 控制刷新率
    drawnow limitrate
    pause(0.001); 
end

% 清理工作区
clear s;