# Taxi-trajectory-analysis
Data Structure Curriculum Design of SCUT CS
# 出租车轨迹分析
## 使用说明：

### 一、数据文件的准备：
##注意：以下说明的压缩包等数据集内容，本项目并不自带。需要时请联系负责该项目的朋友(本项目的Collaborators)。

将Ddata压缩包解压缩到D盘中，此时准备好了文件版本数据的使用。

将Edata压缩包解压缩到E盘中，此时准备好了序列化版本数据的使用。

注意：序列化版本的使用也需要解压缩Data压缩包中的big_homework、taxibase和result三个文件夹

Data内容如下：

taxitotal：存放了taxi轨迹点后端数据信息

taxibase：存放了taxi轨迹点前端数据信息

time_taxidata：存放了时间分布轨迹点信息

pos_taxidata：存放了空间分布轨迹点信息，分为inbeijing和outbeijing两部分

pathdata：存放了轨迹点构成的路径图和按频繁度排序的路径，路径分为二进制存储和txt存储

result：存放了前后端的最终处理结果，用于展示结果

big_homework：存放了主运行界面、后端打包的exe安装文件及安装的位置、输入输出文件实例和示例，bat文件（详见下方）

### 二、运行目录的准备：

将big_homework放入D盘中后，里面有如下结构：

file_use文件夹：下面msi安装包安装到的位置

data_input.txt：输入文件，前端将参数传入其中，后端进行调用以获得结果文件。

data_input_example.txt：使用的样例输入文件，是帮助用户阅读如何传入参数的文件

taxi_exe文件版本.msi：windows安装包，使用时请安装到file_use文件夹下

taxi_exe序列化版本.msi：msi：windows安装包，使用时请安装到file_use文件夹下

Taxi_trajectory_analysis.bat：前端的调用文件，通过点击运行exe可以调用该bat文件执行exe文件的运行。

UI.html：主界面，前端显示文件，尽量不要移动

调用exe.reg：将bat文件写入注册表，使前端网页能够调用。

**在运行前，需要先打开调用exe.reg来注册注册表信息，然后安装taxi_exe文件版本或者taxi_exe序列化版本应用程序到file_use文件夹中，完成前期的准备工作。**

**注意：taxi_exe文件版本和taxi_exe序列化版本只能存在一个，如果想安装另一个，请先在应用中卸载已安装的那个才能安装，卸载和一般软件卸载方式相同。**

### 三、前端网页的说明：

1. 打开UI.html进入网页，在左上角选择要执行的功能，右下角输入或选择相关的参数。
  
2. 如果是F1-F2功能，稍微等待即可，如果出现异常刷新网页即可。
  
3. 如果是F3-F9功能，输入全部参数后，点击确定，网页会请求下载一个data_input.txt文件，将其手动放入D:\big_homework文件夹内部，然后点击执行exe文件，就能在D:\result文件夹获得对应于函数名的生成结果。注意：多次下载data_input.txt时需要进行替换，并保证其文件名为data_input.txt，不能附带(1)(2)这种的格式。执行exe文件可能需要确定。
  
4. 生成的结果如果想要查看，在网页上点击左下角选择文件，在D:\result文件夹内打开对应的函数名文件，即可在网页上看到运行结果。此部分因比较复杂未实现可视化。
  
5. data_input_example文件和result内的rusultexample文件夹内文件为样例展示，可以借鉴这两个文件夹进行参数的输入
