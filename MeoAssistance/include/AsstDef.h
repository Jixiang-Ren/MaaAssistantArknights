#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <ostream>

namespace asst {

	const static std::string Version = "release.beta.02";

	enum class HandleType
	{
		Window = 1,
		View = 2,
		Control = 4
	};

	static std::ostream& operator<<(std::ostream& os, const HandleType& type)
	{
		static std::unordered_map<HandleType, std::string> _type_name = {
			{HandleType::Window, "Window"},
			{HandleType::View, "View"},
			{HandleType::Control, "Control"}
		};
		return os << _type_name.at(type);
	}

	enum class TaskType {
		Invalid = 0,
		BasicClick = 1,
		DoNothing = 2,
		Stop = 4,
		PrintWindow,
		ClickSelf = 8 | BasicClick,
		ClickRect = 16 | BasicClick,
		ClickRand = 32 | BasicClick
	};
	static bool operator&(const TaskType& lhs, const TaskType& rhs) 
	{
		return static_cast<std::underlying_type<TaskType>::type>(lhs) & static_cast<std::underlying_type<TaskType>::type>(rhs);
	}
	static std::ostream& operator<<(std::ostream& os, const TaskType& task)
	{
		static std::unordered_map<TaskType, std::string> _type_name = {
			{TaskType::Invalid, "Invalid"},
			{TaskType::BasicClick, "BasicClick"},
			{TaskType::ClickSelf, "ClickSelf"},
			{TaskType::ClickRect, "ClickRect"},
			{TaskType::ClickRand, "ClickRand"},
			{TaskType::DoNothing, "DoNothing"},
			{TaskType::Stop, "Stop"},
			{TaskType::PrintWindow, "PrintWindow"}
		};
		return os << _type_name.at(task);
	}
	
	enum class AlgorithmType {
		JustReturn,
		MatchTemplate,
		CompareHist
	};
	static std::ostream& operator<<(std::ostream& os, const AlgorithmType& type)
	{
		static std::unordered_map<AlgorithmType, std::string> _type_name = {
			{AlgorithmType::JustReturn, "JustReturn"},
			{AlgorithmType::MatchTemplate, "MatchTemplate"},
			{AlgorithmType::CompareHist, "CompareHist"}
		};
		return os << _type_name.at(type);
	}

	struct Point
	{
		Point() = default;
		Point(int x, int y) : x(x), y(y) {}
		int x = 0;
		int y = 0;
	};

	struct Rect
	{
		Rect() = default;
		Rect(int x, int y, int width, int height)
			: x(x), y(y), width(width), height(height) {}
		Rect operator*(double rhs) const
		{
			return { x, y, static_cast<int>(width * rhs), static_cast<int>(height * rhs) };
		}
		Rect center_zoom(double scale) const
		{
			int half_width_scale = static_cast<int>(width * (1 - scale) / 2);
			int half_hight_scale = static_cast<int>(height * (1 - scale) / 2);
			return { x + half_width_scale, y + half_hight_scale, width - half_width_scale,  height - half_hight_scale };
		}
		int x = 0;
		int y = 0;
		int width = 0;
		int height = 0;
	};

	struct HandleInfo {
		std::string class_name;
		std::string window_name;
	};

	struct AdbCmd {
		std::string path;
		std::string connect;
		std::string click;
		std::string display;
		std::string display_regex;
		int display_width = 0;
		int display_height = 0;
	};

	struct EmulatorInfo {
		std::string name;
		std::vector<HandleInfo> window;
		std::vector<HandleInfo> view;
		std::vector<HandleInfo> control;
		bool is_adb = false;
		AdbCmd adb;
		int width = 0;
		int height = 0;
		int x_offset = 0;
		int y_offset = 0;
		int right_offset = 0;
		int bottom_offset = 0;
	};

	struct TaskInfo {
		std::string filename;							// 图片文件名
		double threshold = 0;							// 模板匹配阈值
		double cache_threshold = 0;						// 直方图比较阈值
		TaskType type = TaskType::Invalid;				// 任务类型
		std::vector<std::string> next;					// 下一个可能的任务（列表）
		int exec_times = 0;								// 任务已执行了多少次
		int max_times = INT_MAX;						// 任务最多执行多少次
		std::vector<std::string> exceeded_next;			// 达到最多次数了之后，下一个可能的任务（列表）
		std::vector<std::string> reduce_other_times;	// 执行了该任务后，需要减少别的任务的执行次数。例如执行了吃理智药，则说明上一次点击蓝色开始行动按钮没生效，所以蓝色开始行动要-1
		asst::Rect specific_area;						// 指定区域，目前仅针对ClickRect任务有用，会点这个区域
		int pre_delay = 0;								// 执行该任务前的延时
		int rear_delay = 0;								// 执行该任务后的延时
	};

	struct Options {
		bool identify_cache = false;	// 图像识别缓存功能：开启后可以大幅降低CPU消耗，但需要保证要识别的按钮每次的位置不会改变
		int identify_delay = 0;			// 图像识别延时：越快操作越快，但会增加CPU消耗
		int control_delay_lower = 0;	// 点击随机延时下限：每次点击操作会进行随机延时
		int control_delay_upper = 0;	// 点击随机延时上限：每次点击操作会进行随机延时
		bool print_window = false;		// 截图功能：开启后每次结算界面会截图到screenshot目录下
		int print_window_delay = 0;		// 截图延时：每次到结算界面，掉落物品不是一次性出来的，有个动画，所以需要等一会再截图
	};
}