// 数据仓库模块.ixx
export module 数据仓库模块;

import 模板模块;
import 主信息定义模块;

export using 世界链类型 = 链表模板<基础信息基类*>;
export inline 世界链类型 世界链{};
export using 特征值链类型 = 链表模板<特征值主信息类*>;
export inline 特征值链类型 特征值链{};
export using 语素链类型 = 链表模板<语素基类*>;
export inline 语素链类型 语素链{};
export using 自然语言链类型 = 链表模板<自然语言成分基类*>;
export inline 自然语言链类型 语言链{};
export using 高级信息链类型 = 链表模板<高级信息基类*>;
export inline 高级信息链类型 行动链{};
