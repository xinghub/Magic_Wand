clc;
clear;
close all;

%% 参数配置
CH_COUNT = 6;                % 实际通道数量
COM_PORT = "COM38";          % 串口号
BAUD_RATE = 921600;          % 波特率
FRAME_SIZE = CH_COUNT*4 + 4; % 每帧字节数
BUFFER_SIZE = 2000;          % 显示数据点数 内存充足时可增大缓冲区
FFT_POINTS = 1024;           % FFT分析点数
PLOT_UPDATE_INTERVAL = 10;   % 图形更新间隔（帧数）显卡性能好时可减小
FFT_UPDATE_INTERVAL = 20;    % FFT更新间隔（帧数）CPU性能好时可减小

%% 串口初始化
try
    s = serialport(COM_PORT, BAUD_RATE);
    configureTerminator(s, 0);
    s.Timeout = 10;
    flush(s); % 清空缓冲区
catch ME
    error('串口连接失败: %s', ME.message);
end

%% 数据缓冲区初始化
circularBuffer = struct(...
    'time', zeros(1, BUFFER_SIZE),...   % 时间戳缓存
    'data', zeros(CH_COUNT, BUFFER_SIZE),... % 数据缓存
    'ptr', 1,...                        % 缓冲区指针
    'count', 0);                        % 有效数据计数

%% 图形界面初始化
figure('Name','实时数据分析','NumberTitle','off',...
    'Position',[100 100 1200 800],...
    'Renderer','painters',...          % 使用更快的渲染器
    'DoubleBuffer','on',...            % 启用双缓冲
    'MenuBar','figure');                 % 隐藏菜单栏

% 预计算FFT频率轴
Fs_expected = BAUD_RATE/(FRAME_SIZE*8); % 理论采样率
f_axis = (0:(FFT_POINTS/2)) * Fs_expected / FFT_POINTS;

% 创建子图和图形对象
hTime = gobjects(CH_COUNT, 1);
hFreq = gobjects(CH_COUNT, 1);
for ch = 1:CH_COUNT
    % 时域子图
    subplot(CH_COUNT,2,ch*2-1);
    hTime(ch) = plot(nan, nan, 'b');
    % 创建右键菜单（每个坐标轴独立）
    cmenu = uicontextmenu;
    uimenu(cmenu, 'Label','保存数据', 'Callback',@(src,evt) saveFigureCallback(ch));
    uimenu(cmenu, 'Label','放大视图', 'Callback',@(src,evt) zoomOn(ch));
    hTime(ch).Parent.UIContextMenu = cmenu; % 绑定到坐标轴
    
    % 启用缩放功能
    zoom(hTime(ch).Parent, 'on');
    
    title(['通道 ',num2str(ch),' 时域波形']);
    xlabel('时间 (s)'); ylabel('幅值');
    grid on;
    xlim([0 5]); % 初始显示5秒窗口
    
    % 频域子图
    subplot(CH_COUNT,2,ch*2);
    hFreq(ch) = plot(f_axis, nan(size(f_axis)), 'r');
    title(['通道 ',num2str(ch),' 频谱']);
    xlabel('频率 (Hz)'); ylabel('幅值');
    grid on;
    xlim([0 Fs_expected/2]);
end

%% 性能优化参数
plotUpdateCounter = 0;      % 图形更新计数器
fftUpdateCounter = 0;       % FFT更新计数器
lastPlotTime = 0;           % 上次绘图时间
Fs_actual = Fs_expected;    % 实际采样率
startTime = tic;            % 全局计时器

%% 主循环
try
    while ishandle(gcf)
        % 批量读取可用数据
        while s.NumBytesAvailable >= FRAME_SIZE
            % 读取并解析单帧数据
            rawData = read(s, FRAME_SIZE, "uint8");
            % 强制转换为 uint8 类型
            rawData = uint8(rawData);
            frameData = typecast(rawData(1:end-4), 'single');
            frameData = double(frameData);
            tail = rawData(end-3:end);
            
            % 校验数据帧
            if isequal(tail, uint8([0x00, 0x00, 0x80, 0x7F]))
                % 解析数据
                frameData = double(typecast(rawData(1:end-4), 'single'));
                currentTime = toc(startTime);
                
                % 更新循环缓冲区
                circularBuffer.time(circularBuffer.ptr) = currentTime;
                circularBuffer.data(:, circularBuffer.ptr) = frameData;
                circularBuffer.ptr = mod(circularBuffer.ptr, BUFFER_SIZE) + 1;
                circularBuffer.count = min(circularBuffer.count + 1, BUFFER_SIZE);
                
                % 更新计数器
                plotUpdateCounter = plotUpdateCounter + 1;
                fftUpdateCounter = fftUpdateCounter + 1;
                
                % 更新时域显示（降低更新频率）
                if plotUpdateCounter >= PLOT_UPDATE_INTERVAL
                    validIdx = getValidIndices(circularBuffer);
                    timeWindow = circularBuffer.time(validIdx);
                    
                    % 更新时域图形数据
                    for ch = 1:CH_COUNT
                        set(hTime(ch),...
                            'XData', timeWindow,...
                            'YData', circularBuffer.data(ch, validIdx));
                    end
                    
                    % 自动调整时间轴
                    timeRange = [max(0, currentTime-5), currentTime];
                    arrayfun(@(h) xlim(h.Parent, timeRange), hTime);
                    
                    plotUpdateCounter = 0;
                end
                
                % 更新频域显示（降低计算频率）
                if fftUpdateCounter >= FFT_UPDATE_INTERVAL && circularBuffer.count >= FFT_POINTS
                    validIdx = getValidIndices(circularBuffer, FFT_POINTS);
                    timeWindow = circularBuffer.time(validIdx);
                    Fs_actual = 1/mean(diff(timeWindow)); % 计算实际采样率
                    
                    % 并行计算各通道FFT
                    for ch = 1:CH_COUNT
                        signal = detrend(circularBuffer.data(ch, validIdx));
                        Y = fft(signal, FFT_POINTS);
                        P2 = abs(Y/FFT_POINTS);
                        P1 = P2(1:FFT_POINTS/2+1);
                        P1(2:end-1) = 2*P1(2:end-1);
                        
                        set(hFreq(ch), 'YData', P1);
                    end
                    fftUpdateCounter = 0;
                end
            else
                warning('无效数据帧：尾部校验失败');
            end
        end
        
        % 控制刷新率（30Hz）
        if toc(startTime) - lastPlotTime > 1/30
            drawnow limitrate nocallbacks;
            lastPlotTime = toc(startTime);
            lastTime = lastPlotTime;
        end
%         pause(0.001);
        fprintf('数据速率: %.1f kbps\n',...
        s.NumBytesAvailable*8/1e3/(toc(startTime)-lastTime));
        % 在图形窗口添加右键菜单示例：
        c = uicontextmenu;
        uimenu(c,'Label','保存数据','Callback',@saveData);
        set(gcf,'UIContextMenu',c);
    end
    
catch ME
    disp('程序异常终止:');
    disp(ME.message);
end

%% 清理资源
clear s;
close all;

%% 辅助函数：获取有效数据索引
function indices = getValidIndices(buffer, requestSize)
    if nargin < 2
        requestSize = buffer.count;
    end
    startIdx = mod(buffer.ptr - requestSize - 1, buffer.count) + 1;
    if startIdx + requestSize - 1 <= buffer.count
        indices = startIdx : startIdx + requestSize - 1;
    else
        indices = [startIdx:buffer.count, 1:requestSize - (buffer.count - startIdx + 1)];
    end
end
%% 新增回调函数（添加到代码末尾）
function saveFigureCallback(ch)
    fig = figure;
    axCopy = copyobj(gca, fig);
    title(axCopy, ['通道 ',num2str(ch),' 波形副本']);
    xlabel(axCopy, '时间 (s)');
    ylabel(axCopy, '幅值');
end

function zoomOn(ch)
    h = subplot(CH_COUNT,2,ch*2-1);
    zoom(h, 'toggle');
end