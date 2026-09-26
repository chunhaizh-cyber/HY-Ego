#include "数据服务.不可变材料.h"
namespace 海中鱼巣 {
namespace {
using Key=L1所有者范围写集本地键;
bool material_budget(const 世界结构预算_B1& b) noexcept { return 世界结构预算有效(b); }
L1所有者范围当前读取结果 读取任一当前事实(
    const L1事实基座服务& l1,稳定编码 id) {
  const L1所有者范围事实读取请求 request{L1所有者范围CRUD合同版本,id};
  const std::array reads{l1.读取所有者范围当前节点(request),
      l1.读取所有者范围当前关系(request),l1.读取所有者范围当前值(request)};
  std::optional<L1所有者范围当前读取结果> hit;
  std::optional<std::uint64_t> observedG;
  for(const auto&read:reads){
    if(read.合同版本!=request.合同版本||read.查询编码!=request.编码||
       (observedG&&*observedG!=read.读取事实代次))
      return {L1所有者范围读取状态::内部不一致,request.合同版本,id,
          read.读取事实代次,std::nullopt};
    observedG=read.读取事实代次;
    if(read.状态==L1所有者范围读取状态::成功){
      if(hit||!read.事实)return {L1所有者范围读取状态::内部不一致,
          request.合同版本,id,read.读取事实代次,std::nullopt};
      hit=read;
    }else{
      if(read.事实)return {L1所有者范围读取状态::内部不一致,
          request.合同版本,id,read.读取事实代次,std::nullopt};
      if(read.状态!=L1所有者范围读取状态::未找到)return read;
    }
  }
  return hit?*hit:reads.front();
}
材料状态_B1 map_read(L1所有者范围读取状态 s) noexcept {
  switch(s){case L1所有者范围读取状态::未找到:return 材料状态_B1::未找到;
  case L1所有者范围读取状态::事实代次漂移:return 材料状态_B1::事实代次漂移;
  case L1所有者范围读取状态::资源失败:return 材料状态_B1::资源失败;
  default:return 材料状态_B1::内部不一致;}
}
材料状态_B1 map_write(L1所有者范围写入状态 s) noexcept {
  switch(s){case L1所有者范围写入状态::事实代次漂移:return 材料状态_B1::事实代次漂移;
  case L1所有者范围写入状态::幂等冲突:return 材料状态_B1::幂等冲突;
  case L1所有者范围写入状态::引用冲突:return 材料状态_B1::引用保护;
  case L1所有者范围写入状态::资源失败:return 材料状态_B1::资源失败;
  default:return 材料状态_B1::内部不一致;}
}
std::array<std::uint8_t,32> sha256(const std::vector<std::uint8_t>& bytes){
  BCRYPT_ALG_HANDLE alg{}; BCRYPT_HASH_HANDLE hash{}; DWORD objectSize{},cb{};
  std::vector<std::uint8_t> object; std::array<std::uint8_t,32> out{};
  if(BCryptOpenAlgorithmProvider(&alg,BCRYPT_SHA256_ALGORITHM,nullptr,0)!=0) throw std::runtime_error("sha");
  try { if(BCryptGetProperty(alg,BCRYPT_OBJECT_LENGTH,reinterpret_cast<PUCHAR>(&objectSize),sizeof(objectSize),&cb,0)!=0) throw std::runtime_error("sha");
    object.resize(objectSize); if(BCryptCreateHash(alg,&hash,object.data(),objectSize,nullptr,0,0)!=0) throw std::runtime_error("sha");
    if(!bytes.empty()&&BCryptHashData(hash,const_cast<PUCHAR>(bytes.data()),static_cast<ULONG>(bytes.size()),0)!=0) throw std::runtime_error("sha");
    if(BCryptFinishHash(hash,out.data(),static_cast<ULONG>(out.size()),0)!=0) throw std::runtime_error("sha");
    BCryptDestroyHash(hash); BCryptCloseAlgorithmProvider(alg,0); return out;
  } catch(...){ if(hash) BCryptDestroyHash(hash); BCryptCloseAlgorithmProvider(alg,0); throw; }
}
std::string hex(const std::array<std::uint8_t,32>& d){ static constexpr char h[]="0123456789abcdef"; std::string s; s.reserve(64); for(auto x:d){s.push_back(h[x>>4]);s.push_back(h[x&15]);} return s; }
std::filesystem::path material_path(const std::filesystem::path& dir,不可变材料格式身份_B1 f,const std::array<std::uint8_t,32>& d){ return dir/(std::to_string(f.值.值)+"-"+hex(d)+".hyb"); }
std::filesystem::path handle_path(HANDLE h){
  const auto n=GetFinalPathNameByHandleW(h,nullptr,0,FILE_NAME_NORMALIZED|VOLUME_NAME_DOS);
  if(!n)throw std::runtime_error("material path");std::wstring s(n,L'\0');
  const auto written=GetFinalPathNameByHandleW(h,s.data(),n,FILE_NAME_NORMALIZED|VOLUME_NAME_DOS);
  if(!written||written>=n)throw std::runtime_error("material path");s.resize(written);
  if(s.rfind(L"\\\\?\\",0)==0)s.erase(0,4);
  return std::filesystem::path(s).lexically_normal();
}
bool same_path(const std::filesystem::path&a,const std::filesystem::path&b)noexcept{
  return _wcsicmp(a.c_str(),b.c_str())==0;
}
HANDLE secure_root(const std::filesystem::path& input,std::filesystem::path& resolved){
  const auto root=std::filesystem::absolute(input).lexically_normal();
  if(!root.is_absolute())throw std::invalid_argument("material root");
  auto current=root.root_path();
  for(const auto&part:root.relative_path()){
    current/=part;
    const auto attrs=GetFileAttributesW(current.c_str());
    if(attrs==INVALID_FILE_ATTRIBUTES){if(!CreateDirectoryW(current.c_str(),nullptr)&&GetLastError()!=ERROR_ALREADY_EXISTS)throw std::runtime_error("material root");}
    const auto actual=GetFileAttributesW(current.c_str());
    if(actual==INVALID_FILE_ATTRIBUTES||(actual&FILE_ATTRIBUTE_DIRECTORY)==0||(actual&FILE_ATTRIBUTE_REPARSE_POINT)!=0)throw std::runtime_error("material root");
  }
  HANDLE h=CreateFileW(root.c_str(),FILE_READ_ATTRIBUTES,FILE_SHARE_READ|FILE_SHARE_WRITE,
    nullptr,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_OPEN_REPARSE_POINT,nullptr);
  if(h==INVALID_HANDLE_VALUE)throw std::runtime_error("material root");
  try{
    FILE_ATTRIBUTE_TAG_INFO tag{};
    if(!GetFileInformationByHandleEx(h,FileAttributeTagInfo,&tag,sizeof(tag))||
       (tag.FileAttributes&FILE_ATTRIBUTE_REPARSE_POINT)!=0)throw std::runtime_error("material root");
    resolved=handle_path(h);if(!same_path(resolved,root))throw std::runtime_error("material root");return h;
  }catch(...){CloseHandle(h);throw;}
}
void require_material_file(HANDLE h,const std::filesystem::path&root){
  FILE_ATTRIBUTE_TAG_INFO tag{};
  if(!GetFileInformationByHandleEx(h,FileAttributeTagInfo,&tag,sizeof(tag))||
     (tag.FileAttributes&FILE_ATTRIBUTE_REPARSE_POINT)!=0||!same_path(handle_path(h).parent_path(),root))
    throw std::runtime_error("material path");
}
std::vector<std::uint8_t> read_file(const std::filesystem::path& root,const std::filesystem::path& p,std::uint64_t max){
  HANDLE h=CreateFileW(p.c_str(),GENERIC_READ,FILE_SHARE_READ,nullptr,OPEN_EXISTING,
                       FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OPEN_REPARSE_POINT,nullptr);
  if(h==INVALID_HANDLE_VALUE)throw std::runtime_error("material");
  try{
    require_material_file(h,root);
    LARGE_INTEGER size{};if(!GetFileSizeEx(h,&size)||size.QuadPart<=0||
       static_cast<std::uint64_t>(size.QuadPart)>max||size.QuadPart>16'777'216)
      throw std::length_error("material");
    std::vector<std::uint8_t> v(static_cast<std::size_t>(size.QuadPart));
    std::size_t at=0;while(at<v.size()){DWORD got{};const auto part=static_cast<DWORD>(
      std::min<std::size_t>(v.size()-at,std::numeric_limits<DWORD>::max()));
      if(!ReadFile(h,v.data()+at,part,&got,nullptr)||got==0)throw std::runtime_error("material");at+=got;}
    std::uint8_t tail{};DWORD got{};if(!ReadFile(h,&tail,1,&got,nullptr)||got!=0)
      throw std::runtime_error("material");CloseHandle(h);return v;
  }catch(...){CloseHandle(h);throw;}
}
bool valid_wire(const std::vector<std::uint8_t>& v,不可变材料格式种类_B1 k){
  if(v.size()<32||v[0]!='H'||v[1]!='Y'||v[2]!='B'||v[3]!='1')return false;
  auto le=[&](std::size_t at,unsigned n){std::uint64_t x=0;for(unsigned i=0;i<n;++i)x|=std::uint64_t(v[at+i])<<(8*i);return x;};
  if(le(4,4)!=1||le(8,8)!=static_cast<std::uint8_t>(k))return false; auto n=le(16,8);
  return n&&n<=(std::numeric_limits<std::size_t>::max()-24)/8&&24+8*n==v.size();
}
结构生命周期_B1 life(std::uint64_t c){return {c};}
L1所有者范围写集请求 material_write_set(
    const 材料发布请求_B1& r, const 不可变材料布局_B1& layout,
    const std::array<std::uint8_t,32>& digest) {
  L1所有者范围写集请求 ws{L1所有者范围CRUD合同版本,r.G0,r.幂等身份};
  ws.节点.push_back({{1},节点种类::普通,std::nullopt});
  ws.关系={{Key{2},layout.族锚点,Key{1},layout.族成员,1},
           {Key{3},Key{1},r.格式.值,layout.材料格式,1},
           {Key{4},Key{1},r.来源,layout.来源,1}};
  std::vector<std::uint64_t> encoded; encoded.reserve(digest.size());
  for(auto b:digest) encoded.push_back(b);
  ws.值={{Key{5},Key{1},layout.字节长度,
          std::int64_t(r.完整载荷.size()),Key{1}},
         {Key{6},Key{1},layout.摘要,std::move(encoded),Key{1}}};
  ws.属性槽变更={{Key{1},layout.字节长度,Key{5}},
                   {Key{1},layout.摘要,Key{6}}};
  return ws;
}
L1所有者范围写集请求 material_exit_set(
    const 材料退出请求_B1& r, const 材料事实_B1& before) {
  L1所有者范围写集请求 ws{L1所有者范围CRUD合同版本,r.G0,r.幂等身份};
  ws.退出事实={before.族关系,before.格式关系,before.来源关系,
               before.长度值,before.摘要值,r.材料.值};
  std::sort(ws.退出事实.begin(),ws.退出事实.end());
  return ws;
}
bool live_at(const L1所有者范围节点事实& f,std::uint64_t h) noexcept {
  return f.创建事实代次&&f.创建事实代次<=h;
}
bool within(const 世界结构用量_B1& u,const 世界结构预算_B1& b) noexcept {
  return u.最大节点数<=b.最大节点数&&u.最大关系数<=b.最大关系数&&u.最大值数<=b.最大值数&&
    u.最大祖先数<=b.最大祖先数&&u.最大后代数<=b.最大后代数&&u.最大候选数<=b.最大候选数&&
    u.最大值元素数<=b.最大值元素数&&u.最大材料字节数<=b.最大材料字节数&&u.最大域原子数<=b.最大域原子数;
}
struct material_meter final {
  世界结构预算_B1 left; 世界结构用量_B1 used{};
  explicit material_meter(const 世界结构预算_B1& b):left(b){}
  世界结构预算_B1 call_budget() const noexcept { return left; }
  bool consume(const 世界结构用量_B1& x) noexcept {
    if(x.最大节点数>left.最大节点数||x.最大关系数>left.最大关系数||x.最大值数>left.最大值数||
       x.最大祖先数>left.最大祖先数||x.最大后代数>left.最大后代数||x.最大候选数>left.最大候选数||
       x.最大值元素数>left.最大值元素数||x.最大材料字节数>left.最大材料字节数||x.最大域原子数>left.最大域原子数)return false;
    auto take=[](std::uint64_t& remaining,std::uint64_t& total,std::uint64_t value){remaining-=value;total+=value;};
    take(left.最大节点数,used.最大节点数,x.最大节点数);
    take(left.最大关系数,used.最大关系数,x.最大关系数);
    take(left.最大值数,used.最大值数,x.最大值数);
    take(left.最大祖先数,used.最大祖先数,x.最大祖先数);
    take(left.最大后代数,used.最大后代数,x.最大后代数);
    take(left.最大候选数,used.最大候选数,x.最大候选数);
    take(left.最大值元素数,used.最大值元素数,x.最大值元素数);
    take(left.最大材料字节数,used.最大材料字节数,x.最大材料字节数);
    take(left.最大域原子数,used.最大域原子数,x.最大域原子数);
    return true;
  }
};
struct material_usage_guard final {
  世界结构用量_B1& target;
  const material_meter& meter;
  void sync() noexcept { target=meter.used; }
  ~material_usage_guard() noexcept { target=meter.used; }
};
L1所有者范围写集请求 original_a_definition_set(std::uint64_t g){
  L1所有者范围写集请求 ws{L1所有者范围CRUD合同版本,g,{1}};
  for(std::uint32_t i=1;i<=16;++i){
    std::optional<L1所有者范围值表示种类> rep;
    if(i==3||i==10||i==14)rep=L1所有者范围值表示种类::U64组;
    else if(i==4||i==9)rep=L1所有者范围值表示种类::I64;
    else if(i==15)rep=L1所有者范围值表示种类::I64组;
    ws.节点.push_back({{i},rep ? 节点种类::属性类型 : 节点种类::普通,rep});
  }
  return ws;
}
L1所有者范围写集请求 material_registration_set(std::uint64_t g,稳定编码 a){
  L1所有者范围写集请求 ws{L1所有者范围CRUD合同版本,g,{0x424D41544C415931ULL}};
  for(std::uint32_t i=1;i<=13;++i){std::optional<L1所有者范围值表示种类> rep;
    if(i==4||i==8||i==11)rep=L1所有者范围值表示种类::I64;else if(i==9)rep=L1所有者范围值表示种类::U64组;
    ws.节点.push_back({{i},rep?节点种类::属性类型:节点种类::普通,rep});}
  for(std::uint32_t i=3;i<=11;++i)ws.关系.push_back({{11+i},Key{1},Key{i},Key{3},static_cast<std::int64_t>(i)});
  ws.关系.push_back({{23},Key{1},a,Key{10},1});
  ws.值.push_back({{24},Key{1},Key{4},std::int64_t{1},Key{1}});ws.属性槽变更.push_back({Key{1},Key{4},Key{24}});
  ws.值.push_back({{25},Key{12},Key{11},std::int64_t{1},Key{12}});ws.属性槽变更.push_back({Key{12},Key{11},Key{25}});
  ws.值.push_back({{26},Key{13},Key{11},std::int64_t{2},Key{13}});ws.属性槽变更.push_back({Key{13},Key{11},Key{26}});
  return ws;
}
bool verify_material_layout(const L1事实基座服务&l,const L1所有者范围写端口&p,
    const 不可变材料布局_B1&layout,std::uint64_t g,材料状态_B1&status,
    std::optional<std::uint64_t>&firstH,世界结构用量_B1&used){
  const auto first=p.读取首次写入材料({L1所有者范围首次写入读取合同版本,{0x424D41544C415931ULL}});
  if(first.读取事实代次!=g){status=材料状态_B1::事实代次漂移;return false;}
  if(first.状态==L1所有者范围读取状态::未找到){status=材料状态_B1::未找到;return false;}
  if(first.状态!=L1所有者范围读取状态::成功){status=map_read(first.状态);return false;}
  if(!first.首次规范化写集||!first.首次写入结果){status=材料状态_B1::内部不一致;return false;}
  const auto expected=material_registration_set(layout.首次G0,layout.A承接锚点);
  const auto&saved=*first.首次写入结果;firstH=saved.事实代次;
  if(*first.首次规范化写集!=expected||saved.所有者!=p.所有者身份()||saved.写入幂等身份.值!=0x424D41544C415931ULL||
     saved.状态!=L1所有者范围写入状态::成功||!saved.是否形成内存权威发布||saved.事实代次!=layout.首次H||saved.新编码映射.size()!=26){status=材料状态_B1::幂等冲突;return false;}
  std::array<稳定编码,26> id{};std::array<bool,26> seen{};
  for(const auto&[key,value]:saved.新编码映射){if(key.值<1||key.值>26||seen[key.值-1]||!有效(value)){status=材料状态_B1::内部不一致;return false;}seen[key.值-1]=true;id[key.值-1]=value;}
  const 不可变材料布局_B1 actual{id[0],id[1],id[2],id[3],id[4],id[5],id[6],id[7],id[8],id[9],id[10],{id[11]},{id[12]},layout.A承接锚点,layout.首次G0,layout.首次H};
  if(actual!=layout){status=材料状态_B1::幂等冲突;return false;}
  const auto owner=p.所有者身份();
  for(std::size_t i=0;i<26;++i){
    if(i<13)++used.最大节点数;else if(i<23)++used.最大关系数;else ++used.最大值数;
    const auto raw=读取任一当前事实(l,id[i]);
    if(raw.读取事实代次!=g){status=材料状态_B1::事实代次漂移;return false;}
    if(raw.状态!=L1所有者范围读取状态::成功||!raw.事实){status=map_read(raw.状态);return false;}
    if(i<13){const auto*n=std::get_if<L1所有者范围节点事实>(&*raw.事实);const bool i64=i==3||i==7||i==10,u64=i==8;
      if(!n||n->写入所有者!=owner||n->创建事实代次!=layout.首次H||n->种类!=(i64||u64?节点种类::属性类型:节点种类::普通)||
         (i64&&n->属性类型表示!=L1所有者范围值表示种类::I64)||(u64&&n->属性类型表示!=L1所有者范围值表示种类::U64组)||(!i64&&!u64&&n->属性类型表示)){status=材料状态_B1::内部不一致;return false;}
      const bool hasSlot=i==0||i==11||i==12;if(n->当前属性.size()!=(hasSlot?1U:0U)){status=材料状态_B1::内部不一致;return false;}
      if(hasSlot){const auto vi=i==0?23U:i==11?24U:25U,ti=i==0?3U:10U;if(n->当前属性[0].属性类型节点!=id[ti]||n->当前属性[0].当前值!=id[vi]){status=材料状态_B1::内部不一致;return false;}}
    }else if(i<23){const auto*e=std::get_if<L1所有者范围关系事实>(&*raw.事实);const bool carry=i==22;
      if(!e||e->写入所有者!=owner||e->源节点!=id[0]||e->目标节点!=(carry?layout.A承接锚点:id[i-11])||e->关系类型节点!=(carry?id[9]:id[2])||e->角色或顺序!=(carry?1:static_cast<std::int64_t>(i-10))||e->创建事实代次!=layout.首次H){status=材料状态_B1::内部不一致;return false;}
    }else{const auto*v=std::get_if<L1所有者范围值事实>(&*raw.事实);const std::size_t j=i-23;const std::array<std::size_t,3> nodes={0,11,12},types={3,10,10};const std::array<std::int64_t,3> values={1,1,2};
      if(!v||v->写入所有者!=owner||v->所属节点!=id[nodes[j]]||v->属性类型节点!=id[types[j]]||v->来源节点!=id[nodes[j]]||
         !std::holds_alternative<std::int64_t>(v->材料)||std::get<std::int64_t>(v->材料)!=values[j]||v->创建事实代次!=layout.首次H){status=材料状态_B1::内部不一致;return false;}
    }
  }
  status=材料状态_B1::已读取;return true;
}
bool original_a_nodes_complete(const 原A特征定义承接事实_B1& f,
                               稳定编码 anchor) noexcept {
  if(!f.Gread||!f.首次G0||f.首次G0==UINT64_MAX||f.首次H!=f.首次G0+1||
     f.首次H>f.Gread||f.元节点[0].编码!=anchor)return false;
  for(std::size_t i=0;i<f.元节点.size();++i){
    const auto& n=f.元节点[i];
    if(!有效(n.编码)||n.生命周期.创建事实代次!=f.首次H)return false;
    for(std::size_t j=0;j<i;++j)if(n.编码==f.元节点[j].编码)return false;
    std::optional<L1所有者范围值表示种类> rep;
    const auto key=i+1;
    if(key==3||key==10||key==14)rep=L1所有者范围值表示种类::U64组;
    else if(key==4||key==9)rep=L1所有者范围值表示种类::I64;
    else if(key==15)rep=L1所有者范围值表示种类::I64组;
    if(n.种类!=(rep ? 节点种类::属性类型 : 节点种类::普通)||n.属性表示!=rep)return false;
  }
  return true;
}
bool material_fact_complete(const 材料事实_B1& m,std::uint64_t g) {
  if(!g||!有效(m.材料)||!有效(m.格式)||!有效(m.族关系)||!有效(m.格式关系)||
     !有效(m.来源关系)||!有效(m.来源)||!有效(m.长度值)||!有效(m.摘要值)||
     !当前有效(m.生命周期,g)||m.字节长度!=m.完整载荷.size()||
     sha256(m.完整载荷)!=m.SHA256||m.格式见证.格式!=m.格式||
     !valid_wire(m.完整载荷,m.格式见证.种类)||
     !有效(m.格式见证.格式节点.编码)||m.格式见证.格式节点.属性表示.has_value()||
     m.格式见证.格式节点.种类!=节点种类::普通||
     !有效(m.格式见证.格式代码.编码)||!有效(m.格式见证.格式代码.类型)||
     m.格式见证.格式代码.所属!=m.格式.值||m.格式见证.格式代码.来源!=m.格式.值||
     m.格式见证.格式节点.生命周期!=m.格式见证.登记生命周期||
     m.格式见证.格式代码.生命周期!=m.格式见证.登记生命周期||
     !当前有效(m.格式见证.登记生命周期,g))return false;
  const auto expected=std::int64_t(static_cast<std::uint8_t>(m.格式见证.种类));
  if((m.格式见证.种类!=不可变材料格式种类_B1::有序I64载荷&&
      m.格式见证.种类!=不可变材料格式种类_B1::有序U64载荷)||
     m.格式见证.格式代码.内容!=std::variant<std::int64_t,std::vector<std::uint64_t>>{expected})return false;
  const std::array<稳定编码,6> ids={m.材料.值,m.族关系,m.格式关系,m.来源关系,m.长度值,m.摘要值};
  for(std::size_t i=0;i<ids.size();++i)for(std::size_t j=0;j<i;++j)if(ids[i]==ids[j])return false;
  return true;
}
}

bool 原A特征定义承接读取结果_B1::成功(
    const 原A特征定义承接读取请求_B1& r)const noexcept {
  return r.版本==1&&r.Gread&&有效(r.预期锚点)&&material_budget(r.预算)&&
    版本==1&&状态==材料状态_B1::已读取&&Gread==r.Gread&&承接&&
    承接->Gread==Gread&&original_a_nodes_complete(*承接,r.预期锚点)&&
    用量.最大节点数>=16&&within(用量,r.预算);
}

bool 不可变材料登记结果_B1::成功(const 不可变材料登记请求_B1& r)const noexcept{
  if(r.版本!=1||!r.G0||r.G0==UINT64_MAX||r.幂等身份.值!=0x424D41544C415931ULL||
     !有效(r.A承接锚点)||!material_budget(r.预算)||版本!=1||
     (状态!=材料状态_B1::已创建&&状态!=材料状态_B1::精确重复)||
     !Gread||!首次H||!布局||发布!=材料发布状态_B1::确认发布||
     布局->首次G0!=r.G0||布局->首次H!=*首次H||*首次H!=r.G0+1||
     布局->A承接锚点!=r.A承接锚点||!A承接||A承接->Gread!=Gread||
     !original_a_nodes_complete(*A承接,r.A承接锚点)||用量.最大节点数<45||
     用量.最大关系数<10||用量.最大值数<3||!within(用量,r.预算))return false;
  const std::array<稳定编码,14> ids={布局->格式锚点,布局->族锚点,布局->类型登记,
    布局->格式版本,布局->族成员,布局->来源,布局->材料格式,布局->字节长度,
    布局->摘要,布局->A承接,布局->载荷格式代码,布局->有序I64格式.值,
    布局->有序U64格式.值,布局->A承接锚点};
  for(std::size_t i=0;i<ids.size();++i){if(!有效(ids[i]))return false;for(std::size_t j=0;j<i;++j)if(ids[i]==ids[j])return false;}
  return true;
}
bool 材料读取结果_B1::成功(const 材料读取请求_B1&r)const noexcept{try{
  if(r.版本!=1||!r.Gread||!有效(r.材料)||!material_budget(r.预算)||
     版本!=1||状态!=材料状态_B1::已读取||Gread!=r.Gread||!材料||材料->材料!=r.材料||
     !material_fact_complete(*材料,Gread)||用量.最大节点数<2||用量.最大关系数<3||用量.最大值数<3||
     用量.最大值元素数<32||用量.最大材料字节数!=材料->完整载荷.size()||
     !within(用量,r.预算))return false;
  return true;
}catch(...){return false;}}
bool 材料格式读取结果_B1::成功(const 材料格式读取请求_B1&r)const noexcept{
  if(r.版本!=1||!r.Gread||!material_budget(r.预算)||
     (r.种类!=不可变材料格式种类_B1::有序I64载荷&&r.种类!=不可变材料格式种类_B1::有序U64载荷))return false;
  return 版本==1&&状态==材料状态_B1::已读取&&Gread==r.Gread&&格式&&格式->种类==r.种类&&
    有效(格式->格式)&&格式->格式节点.编码==格式->格式.值&&有效(格式->格式代码.编码)&&有效(格式->格式代码.类型)&&
    格式->格式代码.所属==格式->格式.值&&
    格式->格式节点.种类==节点种类::普通&&!格式->格式节点.属性表示&&
    格式->格式代码.来源==格式->格式.值&&格式->格式代码.内容==std::variant<std::int64_t,std::vector<std::uint64_t>>{std::int64_t(static_cast<std::uint8_t>(r.种类))}&&
    格式->格式节点.生命周期==格式->格式代码.生命周期&&格式->登记生命周期==格式->格式节点.生命周期&&
    当前有效(格式->登记生命周期,Gread)&&用量.最大节点数==1&&用量.最大值数==1&&within(用量,r.预算);
}
bool 材料发布结果_B1::原操作已确认()const noexcept{try{
  return 版本==1&&原请求&&原请求->版本==1&&原请求->G0&&原请求->G0<UINT64_MAX&&
    有效(原请求->幂等身份)&&有效(原请求->格式)&&有效(原请求->来源)&&material_budget(原请求->预算)&&
    !原请求->完整载荷.empty()&&原请求->完整载荷.size()<=原请求->预算.最大材料字节数&&
    首次H&&*首次H==原请求->G0+1&&发布==材料发布状态_B1::确认发布&&
    (!首次材料||(material_fact_complete(*首次材料,*首次H)&&
      首次材料->生命周期.创建事实代次==*首次H&&首次材料->格式==原请求->格式&&
      首次材料->来源==原请求->来源&&首次材料->完整载荷==原请求->完整载荷&&
      首次材料->字节长度==原请求->完整载荷.size()));
}catch(...){return false;}}
bool 材料发布结果_B1::成功(const 材料发布请求_B1&r)const noexcept{try{
  if(r.版本!=1||!r.G0||r.G0==UINT64_MAX||!有效(r.幂等身份)||!有效(r.格式)||!有效(r.来源)||
     !material_budget(r.预算)||r.完整载荷.empty()||r.完整载荷.size()>r.预算.最大材料字节数||
     版本!=1||!原请求||*原请求!=r||!Gread||!within(用量,r.预算))return false;
  if(状态==材料状态_B1::已复用)return !首次H&&!首次材料&&
    发布==材料发布状态_B1::确认未发布&&!本次来源已记录&&
    当前终态==当前终态_B1::原后态仍成立&&当前材料&&
    material_fact_complete(*当前材料,Gread)&&当前材料->格式==r.格式&&
    当前材料->完整载荷==r.完整载荷;
  if((状态!=材料状态_B1::已创建&&状态!=材料状态_B1::精确重复)||
     !原操作已确认()||Gread<*首次H||!本次来源已记录)return false;
  if(当前终态==当前终态_B1::已删除)
    return 状态==材料状态_B1::精确重复&&!当前材料;
  return 当前终态==当前终态_B1::原后态仍成立&&当前材料&&
    material_fact_complete(*当前材料,Gread)&&当前材料->格式==r.格式&&
    当前材料->完整载荷==r.完整载荷&&
    (!首次材料||当前材料->材料==首次材料->材料);
}catch(...){return false;}}
bool 材料退出结果_B1::原操作已确认()const noexcept{try{
  if(版本!=1||!Gread||!原请求||原请求->版本!=1||!原请求->G0||原请求->G0==UINT64_MAX||
     !有效(原请求->幂等身份)||!有效(原请求->材料)||!material_budget(原请求->预算)||!within(用量,原请求->预算)||
     !首次H||!*首次H||*首次H>Gread||发布!=材料发布状态_B1::确认发布||
     *首次H!=原请求->G0+1||当前终态!=当前终态_B1::已删除)return false;
  return true;
}catch(...){return false;}}
bool 材料退出结果_B1::成功(const 材料退出请求_B1&r)const noexcept{
  return r.版本==1&&r.G0&&r.G0<UINT64_MAX&&有效(r.幂等身份)&&
    有效(r.材料)&&material_budget(r.预算)&&原请求&&*原请求==r&&
    (状态==材料状态_B1::已删除||状态==材料状态_B1::精确重复)&&
    原操作已确认()&&within(用量,r.预算);
}

原A特征定义承接读取结果_B1 不可变材料数据服务::读取原A特征定义承接_B1(
    const 原A特征定义承接读取请求_B1&r,const L1事实基座服务&l,
    const L1所有者范围写端口&p)noexcept{
  原A特征定义承接读取结果_B1 out;out.Gread=r.Gread;
  try{
    if(r.版本!=1||!r.Gread||!有效(r.预期锚点)||!material_budget(r.预算)||
       !p.有效()||!p.绑定于(l))return out;
    const auto first=p.读取首次写入材料({L1所有者范围首次写入读取合同版本,{1}});
    out.Gread=first.读取事实代次;
    if(first.读取事实代次!=r.Gread){out.状态=材料状态_B1::事实代次漂移;return out;}
    if(first.状态==L1所有者范围读取状态::未找到){out.状态=材料状态_B1::未找到;return out;}
    if(first.状态!=L1所有者范围读取状态::成功){out.状态=map_read(first.状态);return out;}
    if(!first.首次规范化写集||!first.首次写入结果){out.状态=材料状态_B1::内部不一致;return out;}
    const auto& ws=*first.首次规范化写集;const auto& saved=*first.首次写入结果;
    if(ws.合同版本!=L1所有者范围CRUD合同版本||ws.写入幂等身份.值!=1||
       ws.期望事实代次==0||ws.期望事实代次==UINT64_MAX||ws!=original_a_definition_set(ws.期望事实代次)){
      out.状态=材料状态_B1::旧格式不支持;return out;
    }
    const auto owner=p.所有者身份();
    if(first.所有者!=owner||first.写入幂等身份.值!=1||saved.合同版本!=L1所有者范围CRUD合同版本||
       saved.所有者!=owner||saved.写入幂等身份.值!=1||saved.状态!=L1所有者范围写入状态::成功||
       !saved.是否形成内存权威发布||saved.重试边界!=L1所有者范围重试边界::不适用||
       saved.事实代次!=ws.期望事实代次+1||saved.事实代次>r.Gread||saved.新编码映射.size()!=16){
      out.状态=材料状态_B1::内部不一致;return out;
    }
    if(r.预算.最大节点数<16){out.状态=材料状态_B1::数量预算不足;return out;}
    const auto ownerRead=l.读取当前结构所有者({L1所有者范围CRUD合同版本,owner});
    out.Gread=ownerRead.读取事实代次;
    if(ownerRead.读取事实代次!=r.Gread){out.状态=材料状态_B1::事实代次漂移;return out;}
    if(ownerRead.状态!=L1所有者范围读取状态::成功||!ownerRead.所有者事实){out.状态=map_read(ownerRead.状态);return out;}
    if(ownerRead.所有者事实->所有者!=owner||ownerRead.所有者事实->范围种类!=L1所有者范围种类::独占结构范围||
       ownerRead.所有者事实->创建事实代次>ws.期望事实代次){
      out.状态=材料状态_B1::内部不一致;return out;
    }
    原A特征定义承接事实_B1 fact{r.Gread,ws.期望事实代次,saved.事实代次,{}};
    std::array<bool,16> seen{};
    for(const auto& [key,id]:saved.新编码映射){
      if(key.值<1||key.值>16||seen[key.值-1]||!有效(id)){out.状态=材料状态_B1::内部不一致;return out;}
      seen[key.值-1]=true;
      for(std::size_t i=0;i<key.值-1;++i)if(fact.元节点[i].编码==id){out.状态=材料状态_B1::内部不一致;return out;}
      ++out.用量.最大节点数;
      const auto raw=读取任一当前事实(l,id);out.Gread=raw.读取事实代次;
      if(raw.读取事实代次!=r.Gread){out.状态=材料状态_B1::事实代次漂移;return out;}
      if(raw.状态==L1所有者范围读取状态::未找到||
         (raw.状态==L1所有者范围读取状态::成功&&!raw.事实)){
        out.状态=材料状态_B1::内部不一致;return out;
      }
      if(raw.状态!=L1所有者范围读取状态::成功){out.状态=map_read(raw.状态);return out;}
      const auto*n=std::get_if<L1所有者范围节点事实>(&*raw.事实);
      if(!n||n->编码!=id||n->写入所有者!=owner||n->创建事实代次!=saved.事实代次){out.状态=材料状态_B1::内部不一致;return out;}
      fact.元节点[key.值-1]={id,n->种类,n->属性类型表示,life(n->创建事实代次)};
    }
    if(fact.元节点[0].编码!=r.预期锚点){out.状态=材料状态_B1::入口拒绝;return out;}
    if(!original_a_nodes_complete(fact,r.预期锚点)){out.状态=材料状态_B1::内部不一致;return out;}
    const auto tail=l.读取中性当前事实代次({L1中性CRUD合同版本});out.Gread=tail.事实代次;
    if(tail.状态!=L1中性读取状态::成功){out.状态=tail.状态==L1中性读取状态::资源失败 ? 材料状态_B1::资源失败 : 材料状态_B1::内部不一致;return out;}
    if(tail.事实代次!=r.Gread){out.状态=材料状态_B1::事实代次漂移;return out;}
    out.承接=std::move(fact);out.状态=材料状态_B1::已读取;return out;
  }catch(const std::bad_alloc&){out.状态=材料状态_B1::资源失败;}
   catch(...){out.状态=材料状态_B1::内部不一致;}
  return out;
}

不可变材料数据服务::不可变材料数据服务(const L1事实基座服务&l,L1所有者范围写端口&&p,
    const 不可变材料布局_B1&x,const std::filesystem::path&d,const L1所有者范围写端口&a)
    :l1_(l),port_(std::move(p)),layout_(x){
  世界结构预算_B1 budget{1'048'576,1'048'576,1'048'576,1'048'576,1'048'576,1'048'576,1'048'576,16'777'216,1'048'576};
  材料状态_B1 reason=材料状态_B1::入口拒绝;std::uint64_t g{};std::optional<std::uint64_t> h;世界结构用量_B1 used{};
  try{
    if(!d.is_absolute()||!port_.有效()||!port_.绑定于(l)||!a.有效()||!a.绑定于(l)||port_.所有者身份()==a.所有者身份()||
       !layout_.首次G0||!layout_.首次H||layout_.首次H!=layout_.首次G0+1||!有效(layout_.A承接锚点))
      throw 材料构造失败_B1(reason,g,h,材料发布状态_B1::确认未发布,used);
    directory_=d.lexically_normal();
    const auto current=l.读取中性当前事实代次({L1中性CRUD合同版本});g=current.事实代次;
    if(current.状态!=L1中性读取状态::成功){reason=current.状态==L1中性读取状态::资源失败 ? 材料状态_B1::资源失败 : 材料状态_B1::内部不一致;throw 材料构造失败_B1(reason,g,h,材料发布状态_B1::确认未发布,used);}
    if(!verify_material_layout(l,port_,layout_,g,reason,h,used))
      throw 材料构造失败_B1(reason,g,h,h ? 材料发布状态_B1::确认发布 : 材料发布状态_B1::确认未发布,used);
    世界结构预算_B1 remaining=budget;
    remaining.最大节点数-=used.最大节点数;remaining.最大关系数-=used.最大关系数;remaining.最大值数-=used.最大值数;
    remaining.最大祖先数-=used.最大祖先数;remaining.最大后代数-=used.最大后代数;remaining.最大候选数-=used.最大候选数;
    remaining.最大值元素数-=used.最大值元素数;remaining.最大材料字节数-=used.最大材料字节数;remaining.最大域原子数-=used.最大域原子数;
    const 原A特征定义承接读取请求_B1 ar{1,g,layout_.A承接锚点,remaining};
    const auto proof=读取原A特征定义承接_B1(ar,l,a);
    used.最大节点数+=proof.用量.最大节点数;used.最大关系数+=proof.用量.最大关系数;used.最大值数+=proof.用量.最大值数;
    used.最大祖先数+=proof.用量.最大祖先数;used.最大后代数+=proof.用量.最大后代数;used.最大候选数+=proof.用量.最大候选数;
    used.最大值元素数+=proof.用量.最大值元素数;used.最大材料字节数+=proof.用量.最大材料字节数;used.最大域原子数+=proof.用量.最大域原子数;
    if(!proof.成功(ar)){reason=proof.状态;throw 材料构造失败_B1(reason,proof.Gread,h,材料发布状态_B1::确认发布,used);}
    directory_=d.lexically_normal();
    HANDLE root=secure_root(directory_,directory_);directory_handle_=root;
    const auto tail=l.读取中性当前事实代次({L1中性CRUD合同版本});g=tail.事实代次;
    if(tail.状态!=L1中性读取状态::成功||tail.事实代次!=proof.Gread){
      CloseHandle(root);directory_handle_=nullptr;
      reason=tail.状态==L1中性读取状态::资源失败 ? 材料状态_B1::资源失败 :
        tail.状态==L1中性读取状态::成功 ? 材料状态_B1::事实代次漂移 : 材料状态_B1::内部不一致;
      throw 材料构造失败_B1(reason,g,h,材料发布状态_B1::确认发布,used);
    }
  }catch(const 材料构造失败_B1&){throw;}
   catch(const std::bad_alloc&){throw 材料构造失败_B1(材料状态_B1::资源失败,g,h,h?材料发布状态_B1::确认发布:材料发布状态_B1::确认未发布,used);}
   catch(const std::length_error&){throw 材料构造失败_B1(材料状态_B1::资源失败,g,h,h?材料发布状态_B1::确认发布:材料发布状态_B1::确认未发布,used);}
   catch(...){throw 材料构造失败_B1(材料状态_B1::内部不一致,g,h,h?材料发布状态_B1::确认发布:材料发布状态_B1::确认未发布,used);}
}
不可变材料数据服务::~不可变材料数据服务() noexcept {
  if(directory_handle_)CloseHandle(static_cast<HANDLE>(directory_handle_));
}
bool 不可变材料数据服务::绑定于(const L1事实基座服务&l)const noexcept{return port_.绑定于(l);}

不可变材料登记结果_B1 不可变材料数据服务::登记不可变材料结构_B1(const 不可变材料登记请求_B1&r,L1事实基座服务&l,L1所有者范围写端口&p,const L1所有者范围写端口&a)noexcept{
  不可变材料登记结果_B1 out; out.Gread=r.G0;
  try{
    if(r.版本!=1||!r.G0||r.G0==UINT64_MAX||r.幂等身份.值!=0x424D41544C415931ULL||!有效(r.A承接锚点)||!material_budget(r.预算)||!p.有效()||!p.绑定于(l)||!a.有效()||!a.绑定于(l)||p.所有者身份()==a.所有者身份())return out;
    if(r.预算.最大节点数<45||r.预算.最大关系数<10||r.预算.最大值数<3){out.状态=材料状态_B1::数量预算不足;return out;}
    L1所有者范围写集请求 ws{L1所有者范围CRUD合同版本,r.G0,r.幂等身份};
    for(std::uint32_t i=1;i<=13;++i){std::optional<L1所有者范围值表示种类> rep; if(i==4||i==8||i==11)rep=L1所有者范围值表示种类::I64; else if(i==9)rep=L1所有者范围值表示种类::U64组; ws.节点.push_back({{i},rep ? 节点种类::属性类型 : 节点种类::普通,rep});}
    for(std::uint32_t i=3;i<=11;++i)ws.关系.push_back({{11+i},Key{1},Key{i},Key{3},static_cast<std::int64_t>(i)});
    ws.关系.push_back({{23},Key{1},r.A承接锚点,Key{10},1});
    ws.值.push_back({{24},Key{1},Key{4},std::int64_t{1},Key{1}}); ws.属性槽变更.push_back({Key{1},Key{4},Key{24}});
    ws.值.push_back({{25},Key{12},Key{11},std::int64_t{1},Key{12}}); ws.属性槽变更.push_back({Key{12},Key{11},Key{25}});
    ws.值.push_back({{26},Key{13},Key{11},std::int64_t{2},Key{13}}); ws.属性槽变更.push_back({Key{13},Key{11},Key{26}});
    const auto first=p.读取首次写入材料({L1所有者范围首次写入读取合同版本,r.幂等身份}); bool replay=false;
    if(first.状态==L1所有者范围读取状态::成功){
      replay=true;
      if(!first.首次规范化写集||!first.首次写入结果){out.状态=材料状态_B1::旧格式不支持;return out;}
      out.首次H=first.首次写入结果->事实代次;out.发布=材料发布状态_B1::确认发布;
      const auto& old=*first.首次规范化写集;
      稳定编码 oldAnchor{};
      for(const auto& edge:old.关系)if(edge.本地键.值==23){
        if(const auto* external=std::get_if<稳定编码>(&edge.目标节点))oldAnchor=*external;
      }
      if(!有效(oldAnchor)||old!=material_registration_set(old.期望事实代次,oldAnchor)){
        out.状态=材料状态_B1::旧格式不支持;return out;
      }
      if(old.期望事实代次!=r.G0||oldAnchor!=r.A承接锚点){
        out.状态=材料状态_B1::幂等冲突;out.Gread=first.读取事实代次;return out;
      }
      ws=old;
    }
    else if(first.状态==L1所有者范围读取状态::未找到){
      if(first.读取事实代次!=r.G0){out.状态=材料状态_B1::事实代次漂移;out.Gread=first.读取事实代次;return out;}
      const L1所有者范围空域完整读取请求_v2 emptyRequest{
          L1所有者范围空域完整读取合同版本,p.所有者身份(),r.G0};
      const auto empty=l.读取所有者范围完整空域(emptyRequest);
      const bool emptyOk=empty.状态==L1所有者范围空域完整读取状态_v2::成功
          &&empty.合同版本==emptyRequest.合同版本&&empty.所有者==emptyRequest.所有者
          &&empty.期望事实代次==emptyRequest.期望事实代次
          &&empty.读取事实代次==emptyRequest.期望事实代次
          &&empty.空域&&*empty.空域;
      if(!emptyOk){out.状态=empty.状态==L1所有者范围空域完整读取状态_v2::资源失败
          ?材料状态_B1::资源失败:材料状态_B1::旧格式不支持;out.Gread=empty.读取事实代次;return out;}}
    else{out.状态=map_read(first.状态);out.Gread=first.读取事实代次;return out;}
    世界结构预算_B1 aBudget=r.预算;aBudget.最大节点数-=13;aBudget.最大关系数-=10;aBudget.最大值数-=3;
    const 原A特征定义承接读取请求_B1 ar{1,first.读取事实代次,r.A承接锚点,aBudget};
    const auto proof=读取原A特征定义承接_B1(ar,l,a);out.Gread=proof.Gread;out.用量=proof.用量;
    if(!proof.成功(ar)){out.状态=proof.状态;return out;}
    out.A承接=proof.承接;
    out.发布=材料发布状态_B1::可能发布; const auto saved=p.提交所有者范围中性写集(ws);out.Gread=saved.事实代次;
    if(saved.状态!=(replay ? L1所有者范围写入状态::精确重复 : L1所有者范围写入状态::成功)){out.状态=map_write(saved.状态);return out;}
    out.首次H=saved.事实代次;out.发布=材料发布状态_B1::确认发布;
    std::array<稳定编码,26> id{};
    for(auto[k,v]:saved.新编码映射)if(k.值>=1&&k.值<=26)id[k.值-1]=v;
    for(auto v:id)if(!有效(v)){out.状态=材料状态_B1::可能已发布;return out;}
    const auto owner=p.所有者身份();
    const auto h=saved.事实代次;const auto verifyG=replay ? first.读取事实代次 : saved.事实代次;
    auto read=[&](稳定编码 value)->std::optional<L1所有者范围事实副本>{
      const auto x=读取任一当前事实(l,value);
      out.Gread=x.读取事实代次;
      if(x.读取事实代次!=verifyG||x.状态!=L1所有者范围读取状态::成功||!x.事实)return std::nullopt;
      return x.事实;
    };
    for(std::size_t i=0;i<13;++i){
      ++out.用量.最大节点数;
      const auto raw=read(id[i]);if(!raw){out.状态=材料状态_B1::可能已发布;return out;}
      const auto*n=std::get_if<L1所有者范围节点事实>(&*raw);
      const bool i64=i==3||i==7||i==10,u64=i==8;
      if(!n||n->写入所有者!=owner||n->创建事实代次!=h||
         n->种类!=(i64||u64 ? 节点种类::属性类型 : 节点种类::普通)||
         (i64&&n->属性类型表示!=L1所有者范围值表示种类::I64)||
         (u64&&n->属性类型表示!=L1所有者范围值表示种类::U64组)||
         (!i64&&!u64&&n->属性类型表示)){out.状态=材料状态_B1::内部不一致;return out;}
      const bool hasSlot=i==0||i==11||i==12;
      if(n->当前属性.size()!=(hasSlot?1U:0U)){out.状态=材料状态_B1::内部不一致;return out;}
      if(hasSlot){const auto valueIndex=i==0?23U:i==11?24U:25U;
        const auto typeIndex=i==0?3U:10U;
        if(n->当前属性.front().属性类型节点!=id[typeIndex]||n->当前属性.front().当前值!=id[valueIndex]){out.状态=材料状态_B1::内部不一致;return out;}}
    }
    for(std::size_t i=0;i<9;++i){
      ++out.用量.最大关系数;
      const auto raw=read(id[13+i]);if(!raw){out.状态=材料状态_B1::可能已发布;return out;}
      const auto*e=std::get_if<L1所有者范围关系事实>(&*raw);
      if(!e||e->写入所有者!=owner||e->源节点!=id[0]||e->目标节点!=id[i+2]||
         e->关系类型节点!=id[2]||e->角色或顺序!=static_cast<std::int64_t>(i+3)||
         e->创建事实代次!=h){out.状态=材料状态_B1::内部不一致;return out;}
    }
    {
      ++out.用量.最大关系数;
      const auto raw=read(id[22]);if(!raw){out.状态=材料状态_B1::可能已发布;return out;}
      const auto*e=std::get_if<L1所有者范围关系事实>(&*raw);
      if(!e||e->写入所有者!=owner||e->源节点!=id[0]||e->目标节点!=r.A承接锚点||
         e->关系类型节点!=id[9]||e->角色或顺序!=1||e->创建事实代次!=h){out.状态=材料状态_B1::内部不一致;return out;}
    }
    const std::array<std::int64_t,3> expectedValues={1,1,2};
    const std::array<std::size_t,3> valueNodes={0,11,12};
    const std::array<std::size_t,3> valueTypes={3,10,10};
    for(std::size_t i=0;i<3;++i){
      ++out.用量.最大值数;
      const auto raw=read(id[23+i]);if(!raw){out.状态=材料状态_B1::可能已发布;return out;}
      const auto*v=std::get_if<L1所有者范围值事实>(&*raw);
      if(!v||v->写入所有者!=owner||v->所属节点!=id[valueNodes[i]]||
         v->属性类型节点!=id[valueTypes[i]]||v->来源节点!=id[valueNodes[i]]||
         !std::holds_alternative<std::int64_t>(v->材料)||std::get<std::int64_t>(v->材料)!=expectedValues[i]||
         v->创建事实代次!=h){out.状态=材料状态_B1::内部不一致;return out;}
    }
    世界结构预算_B1 finalBudget=r.预算;
    finalBudget.最大节点数-=29;finalBudget.最大关系数-=10;finalBudget.最大值数-=3;
    const 原A特征定义承接读取请求_B1 finalRequest{1,verifyG,r.A承接锚点,finalBudget};
    const auto finalProof=读取原A特征定义承接_B1(finalRequest,l,a);out.Gread=finalProof.Gread;
    out.用量.最大节点数+=finalProof.用量.最大节点数;out.用量.最大关系数+=finalProof.用量.最大关系数;out.用量.最大值数+=finalProof.用量.最大值数;
    if(!finalProof.成功(finalRequest)){out.状态=finalProof.状态;return out;}
    out.A承接=finalProof.承接;
    out.首次H=h;
    out.布局=不可变材料布局_B1{id[0],id[1],id[2],id[3],id[4],id[5],id[6],id[7],id[8],id[9],id[10],{id[11]},{id[12]},r.A承接锚点,ws.期望事实代次,h};
    out.状态=replay ? 材料状态_B1::精确重复 : 材料状态_B1::已创建;
    out.发布=材料发布状态_B1::确认发布;return out;
  }catch(const std::bad_alloc&){out.状态=out.发布==材料发布状态_B1::可能发布 ? 材料状态_B1::可能已发布 : 材料状态_B1::资源失败;}catch(...){out.状态=out.发布==材料发布状态_B1::可能发布 ? 材料状态_B1::可能已发布 : 材料状态_B1::内部不一致;}return out;
}

材料格式读取结果_B1 不可变材料数据服务::读取材料格式(const 材料格式读取请求_B1&r)const noexcept{
  材料格式读取结果_B1 out; out.Gread=r.Gread;
  try{
    if(r.版本!=1||!r.Gread||!material_budget(r.预算))return out;
    if(r.预算.最大节点数<1||r.预算.最大值数<1){out.状态=材料状态_B1::数量预算不足;return out;}
    const auto id=r.种类==不可变材料格式种类_B1::有序I64载荷 ? layout_.有序I64格式.值 :
      r.种类==不可变材料格式种类_B1::有序U64载荷 ? layout_.有序U64格式.值 : 稳定编码{};
    if(!有效(id)){out.状态=材料状态_B1::规则未提供;return out;}
    const auto raw=读取任一当前事实(l1_,id);
    out.用量.最大节点数=1;
    out.Gread=raw.读取事实代次;
    if(raw.读取事实代次!=r.Gread){out.状态=材料状态_B1::事实代次漂移;return out;}
    if(raw.状态!=L1所有者范围读取状态::成功||!raw.事实){out.状态=map_read(raw.状态);return out;}
    const auto*n=std::get_if<L1所有者范围节点事实>(&*raw.事实);
    if(!n||n->写入所有者!=port_.所有者身份()||n->种类!=节点种类::普通||n->属性类型表示||n->创建事实代次!=layout_.首次H){out.状态=材料状态_B1::内部不一致;return out;}
    if(n->创建事实代次>r.Gread){out.状态=材料状态_B1::内部不一致;return out;}
    const L1所有者范围所属节点当前完整值组读取请求_v2 valuesRequest{
      L1所有者范围所属节点当前完整值组读取合同版本_v2,
      port_.所有者身份(),id,r.Gread};
    const auto values=l1_.读取所有者范围所属节点当前完整值组(valuesRequest);
    out.用量.最大值数=1;
    if(values.读取事实代次!=r.Gread){out.状态=材料状态_B1::事实代次漂移;return out;}
    if(values.状态!=L1所有者范围所属节点当前完整值组读取状态_v2::成功){out.状态=材料状态_B1::内部不一致;return out;}
    if(values.载荷.size()!=1){out.状态=材料状态_B1::内部不一致;return out;}
    const auto&v=values.载荷.front(); const auto expected=static_cast<std::int64_t>(static_cast<std::uint8_t>(r.种类));
    if(v.写入所有者!=port_.所有者身份()||v.所属节点!=id||v.属性类型节点!=layout_.载荷格式代码||v.来源节点!=id||!std::holds_alternative<std::int64_t>(v.材料)||std::get<std::int64_t>(v.材料)!=expected||v.创建事实代次!=n->创建事实代次){out.状态=材料状态_B1::内部不一致;return out;}
    const 结构生命周期_B1 nl=life(n->创建事实代次),vl=life(v.创建事实代次);
    out.格式=材料格式事实_B1{{id},r.种类,{id,n->种类,n->属性类型表示,nl},{v.编码,v.所属节点,v.属性类型节点,v.来源节点,expected,vl},nl};
    const auto tail=l1_.读取中性当前事实代次({L1中性CRUD合同版本});out.Gread=tail.事实代次;
    if(tail.状态!=L1中性读取状态::成功){out.状态=tail.状态==L1中性读取状态::资源失败 ? 材料状态_B1::资源失败 : 材料状态_B1::内部不一致;return out;}
    if(tail.事实代次!=r.Gread){out.状态=材料状态_B1::事实代次漂移;return out;}
    out.状态=材料状态_B1::已读取; return out;
  }catch(const std::bad_alloc&){out.状态=材料状态_B1::资源失败;}catch(...){out.状态=材料状态_B1::内部不一致;}return out;
}

不可变材料数据服务::材料完整读取内部结果_B1
不可变材料数据服务::读取材料完整内部(const 材料读取请求_B1&r)const noexcept{
  材料完整读取内部结果_B1 out;
  auto& publicOut=out.公开结果; publicOut.Gread=r.Gread;
  material_meter meter(r.预算);
  material_usage_guard usage{publicOut.用量,meter};
  struct incomplete_evidence_guard final {
    材料完整读取内部结果_B1& value;
    void sync() noexcept {
      if(value.公开结果.状态!=材料状态_B1::已读取){value.关系.clear();value.属性.clear();}
    }
    ~incomplete_evidence_guard() noexcept { sync(); }
  } evidenceGuard{out};
  auto finish=[&]() -> 材料完整读取内部结果_B1 { usage.sync(); evidenceGuard.sync(); return std::move(out); };
  try{
    if(r.版本!=1||!r.Gread||!有效(r.材料)||!material_budget(r.预算))return finish();
    if(!meter.consume({1,0,0,0,0,0,0,0,0})){publicOut.状态=材料状态_B1::数量预算不足;return finish();}
    const auto raw=读取任一当前事实(l1_,r.材料.值);publicOut.Gread=raw.读取事实代次;
    if(raw.读取事实代次!=r.Gread){publicOut.状态=材料状态_B1::事实代次漂移;return finish();}
    if(raw.状态!=L1所有者范围读取状态::成功||!raw.事实){publicOut.状态=map_read(raw.状态);return finish();}
    const auto*n=std::get_if<L1所有者范围节点事实>(&*raw.事实);if(!n||n->写入所有者!=port_.所有者身份()||n->种类!=节点种类::普通){publicOut.状态=材料状态_B1::内部不一致;return finish();}
    const auto nl=life(n->创建事实代次);
    if(n->创建事实代次>r.Gread){publicOut.状态=材料状态_B1::内部不一致;return finish();}
    if(!meter.consume({0,3,2,0,0,0,32,0,0})){publicOut.状态=材料状态_B1::数量预算不足;return finish();}
    const auto edges=l1_.读取所有者范围当前目标关系组({L1所有者范围CRUD合同版本,r.材料.值,layout_.族成员});
    publicOut.Gread=edges.读取事实代次;if(edges.读取事实代次!=r.Gread){publicOut.状态=材料状态_B1::事实代次漂移;return finish();}
    if(edges.状态!=L1所有者范围读取状态::成功){publicOut.状态=map_read(edges.状态);return finish();}
    const auto fmt=l1_.读取所有者范围当前源关系组({L1所有者范围CRUD合同版本,r.材料.值,layout_.材料格式});
    publicOut.Gread=fmt.读取事实代次;if(fmt.读取事实代次!=r.Gread){publicOut.状态=材料状态_B1::事实代次漂移;return finish();}
    if(fmt.状态!=L1所有者范围读取状态::成功){publicOut.状态=map_read(fmt.状态);return finish();}
    const auto src=l1_.读取所有者范围当前源关系组({L1所有者范围CRUD合同版本,r.材料.值,layout_.来源});
    publicOut.Gread=src.读取事实代次;if(src.读取事实代次!=r.Gread){publicOut.状态=材料状态_B1::事实代次漂移;return finish();}
    if(src.状态!=L1所有者范围读取状态::成功){publicOut.状态=map_read(src.状态);return finish();}
    const L1所有者范围所属节点当前完整值组读取请求_v2 valsRequest{
      L1所有者范围所属节点当前完整值组读取合同版本_v2,
      port_.所有者身份(),r.材料.值,r.Gread};
    const auto vals=l1_.读取所有者范围所属节点当前完整值组(valsRequest);
    publicOut.Gread=vals.读取事实代次;if(vals.读取事实代次!=r.Gread){publicOut.状态=材料状态_B1::事实代次漂移;return finish();}
    if(vals.状态!=L1所有者范围所属节点当前完整值组读取状态_v2::成功){publicOut.状态=材料状态_B1::内部不一致;return finish();}
    if(edges.关系组.size()!=1||fmt.关系组.size()!=1||src.关系组.size()!=1||vals.载荷.size()!=2){publicOut.状态=材料状态_B1::内部不一致;return finish();}
    const auto&e1=edges.关系组.front();const auto&e2=fmt.关系组.front();const auto&e3=src.关系组.front();
    const auto owner=port_.所有者身份();
    if(e1.写入所有者!=owner||e2.写入所有者!=owner||e3.写入所有者!=owner||
       e1.源节点!=layout_.族锚点||e1.目标节点!=r.材料.值||e1.关系类型节点!=layout_.族成员||e1.角色或顺序!=1||
       e2.源节点!=r.材料.值||e2.关系类型节点!=layout_.材料格式||e2.角色或顺序!=1||
       e3.源节点!=r.材料.值||e3.关系类型节点!=layout_.来源||e3.角色或顺序!=1||
       e1.创建事实代次!=n->创建事实代次||e2.创建事实代次!=n->创建事实代次||e3.创建事实代次!=n->创建事实代次){publicOut.状态=材料状态_B1::内部不一致;return finish();}
    const L1所有者范围值事实 *length=nullptr,*digest=nullptr;for(const auto&v:vals.载荷){if(v.属性类型节点==layout_.字节长度)length=&v;else if(v.属性类型节点==layout_.摘要)digest=&v;}
    if(!length||!digest||length->写入所有者!=owner||digest->写入所有者!=owner||
       length->所属节点!=r.材料.值||digest->所属节点!=r.材料.值||
       length->来源节点!=r.材料.值||digest->来源节点!=r.材料.值||
       length->创建事实代次!=n->创建事实代次||digest->创建事实代次!=n->创建事实代次||
       !std::holds_alternative<std::int64_t>(length->材料)||!std::holds_alternative<std::vector<std::uint64_t>>(digest->材料)){publicOut.状态=材料状态_B1::内部不一致;return finish();}
    const auto len=std::get<std::int64_t>(length->材料);const auto&dv=std::get<std::vector<std::uint64_t>>(digest->材料);if(len<1||dv.size()!=32){publicOut.状态=材料状态_B1::内部不一致;return finish();}
    std::array<std::uint8_t,32>d{};for(std::size_t i=0;i<32;++i){if(dv[i]>255){publicOut.状态=材料状态_B1::内部不一致;return finish();}d[i]=static_cast<std::uint8_t>(dv[i]);}
    不可变材料格式种类_B1 kind;if(e2.目标节点==layout_.有序I64格式.值)kind=不可变材料格式种类_B1::有序I64载荷;else if(e2.目标节点==layout_.有序U64格式.值)kind=不可变材料格式种类_B1::有序U64载荷;else{publicOut.状态=材料状态_B1::规则未提供;return finish();}
    if(meter.left.最大节点数<1||meter.left.最大值数<1){publicOut.状态=材料状态_B1::数量预算不足;return finish();}
    const 材料格式读取请求_B1 formatRequest{1,r.Gread,kind,meter.call_budget()};
    const auto fr=读取材料格式(formatRequest);publicOut.Gread=fr.Gread;
    if(!meter.consume(fr.用量)){publicOut.状态=材料状态_B1::数量预算不足;return finish();}
    if(!fr.成功(formatRequest)){publicOut.状态=fr.状态;return finish();}
    if(static_cast<std::uint64_t>(len)>meter.left.最大材料字节数){publicOut.状态=材料状态_B1::数量预算不足;return finish();}
    auto bytes=read_file(directory_,material_path(directory_,{e2.目标节点},d),meter.left.最大材料字节数);
    if(!meter.consume({0,0,0,0,0,0,0,bytes.size(),0})){publicOut.状态=材料状态_B1::数量预算不足;return finish();}
    if(bytes.size()!=static_cast<std::size_t>(len)||sha256(bytes)!=d||!valid_wire(bytes,kind)){publicOut.状态=材料状态_B1::内部不一致;return finish();}
    const auto tail=l1_.读取中性当前事实代次({L1中性CRUD合同版本});if(tail.状态!=L1中性读取状态::成功){publicOut.状态=tail.状态==L1中性读取状态::资源失败 ? 材料状态_B1::资源失败 : 材料状态_B1::内部不一致;return finish();}if(tail.事实代次!=r.Gread){publicOut.状态=材料状态_B1::事实代次漂移;publicOut.Gread=tail.事实代次;return finish();}
    publicOut.用量=meter.used;
    out.关系={
      {e1.编码,e1.源节点,e1.目标节点,e1.关系类型节点,e1.角色或顺序,life(e1.创建事实代次)},
      {e2.编码,e2.源节点,e2.目标节点,e2.关系类型节点,e2.角色或顺序,life(e2.创建事实代次)},
      {e3.编码,e3.源节点,e3.目标节点,e3.关系类型节点,e3.角色或顺序,life(e3.创建事实代次)}};
    out.属性={
      {length->编码,length->所属节点,length->属性类型节点,length->来源节点,std::get<std::int64_t>(length->材料),life(length->创建事实代次)},
      {digest->编码,digest->所属节点,digest->属性类型节点,digest->来源节点,std::get<std::vector<std::uint64_t>>(digest->材料),life(digest->创建事实代次)}};
    publicOut.材料=材料事实_B1{r.材料,{e2.目标节点},e1.编码,e2.编码,e3.编码,e3.目标节点,length->编码,digest->编码,static_cast<std::uint64_t>(len),d,nl,std::move(bytes),*fr.格式};publicOut.状态=材料状态_B1::已读取;return finish();
  }catch(const std::length_error&){publicOut.状态=材料状态_B1::数量预算不足;}catch(const std::filesystem::filesystem_error&){publicOut.状态=材料状态_B1::资源失败;}catch(const std::bad_alloc&){publicOut.状态=材料状态_B1::资源失败;}catch(...){publicOut.状态=材料状态_B1::内部不一致;}return finish();
}

材料读取结果_B1 不可变材料数据服务::读取材料(const 材料读取请求_B1&r)const noexcept{
  return 读取材料完整内部(r).公开结果;
}

材料发布结果_B1 不可变材料数据服务::发布材料(const 材料发布请求_B1&r)noexcept{
  材料发布结果_B1 out; bool entered=false;
  material_meter meter(r.预算);
  material_usage_guard usage{out.用量,meter};
  auto finish=[&]() -> 材料发布结果_B1 { usage.sync(); return std::move(out); };
  try{
    out.原请求=r; out.Gread=r.G0;
    std::scoped_lock lock(mutex_);
    if(r.版本!=1||!r.G0||r.G0==UINT64_MAX||!有效(r.幂等身份)||!有效(r.格式)||
       !有效(r.来源)||!material_budget(r.预算)||r.完整载荷.empty())return finish();
    if(r.完整载荷.size()>r.预算.最大材料字节数){out.状态=材料状态_B1::数量预算不足;return finish();}
    const auto kind=r.格式==layout_.有序I64格式 ? 不可变材料格式种类_B1::有序I64载荷 :
      r.格式==layout_.有序U64格式 ? 不可变材料格式种类_B1::有序U64载荷 :
      static_cast<不可变材料格式种类_B1>(0);
    if(static_cast<std::uint8_t>(kind)==0||!valid_wire(r.完整载荷,kind)){
      out.状态=材料状态_B1::规则未提供;return finish();
    }
    const auto digest=sha256(r.完整载荷);
    const auto expected=material_write_set(r,layout_,digest);
    const auto first=port_.读取首次写入材料({L1所有者范围首次写入读取合同版本,r.幂等身份});
    out.Gread=first.读取事实代次;
    if(first.状态==L1所有者范围读取状态::成功){
      if(!first.首次规范化写集||!first.首次写入结果){out.状态=材料状态_B1::可能已发布;out.发布=材料发布状态_B1::可能发布;return finish();}
      out.首次H=first.首次写入结果->事实代次;
      if(*first.首次规范化写集!=expected){out.状态=材料状态_B1::幂等冲突;return finish();}
      稳定编码 mid{};for(auto[k,id]:first.首次写入结果->新编码映射)if(k.值==1)mid=id;
      if(!有效(mid)||!*out.首次H){out.状态=材料状态_B1::可能已发布;out.发布=材料发布状态_B1::可能发布;return finish();}
      if(meter.left.最大节点数<2||meter.left.最大关系数<3||meter.left.最大值数<3||meter.left.最大值元素数<32){out.状态=材料状态_B1::数量预算不足;return finish();}
      const 材料读取请求_B1 firstRequest{1,first.读取事实代次,{mid},meter.call_budget()};
      const auto original=读取材料(firstRequest);
      out.Gread=original.Gread;
      if(!meter.consume(original.用量)){out.状态=材料状态_B1::数量预算不足;return finish();}
      if(original.成功(firstRequest)){
        if(original.材料->格式!=r.格式||original.材料->来源!=r.来源||original.材料->完整载荷!=r.完整载荷){out.状态=材料状态_B1::幂等冲突;return finish();}
        out.首次材料=out.当前材料=original.材料;
        out.当前终态=当前终态_B1::原后态仍成立;
      }else if(original.状态==材料状态_B1::未找到){
        out.当前终态=当前终态_B1::已删除;
      }else{
        out.状态=original.状态;out.发布=材料发布状态_B1::可能发布;return finish();
      }
      out.用量=meter.used;out.状态=材料状态_B1::精确重复;out.发布=材料发布状态_B1::确认发布;out.本次来源已记录=true;return finish();
    }
    if(first.状态!=L1所有者范围读取状态::未找到){out.状态=map_read(first.状态);return finish();}
    if(first.读取事实代次!=r.G0){out.状态=材料状态_B1::事实代次漂移;return finish();}

    if(meter.left.最大节点数<1||meter.left.最大值数<1){out.状态=材料状态_B1::数量预算不足;return finish();}
    const 材料格式读取请求_B1 formatRequest{1,r.G0,kind,meter.call_budget()};
    const auto format=读取材料格式(formatRequest);
    if(!meter.consume(format.用量)){out.状态=材料状态_B1::数量预算不足;return finish();}
    if(!format.成功(formatRequest)){out.状态=format.状态;out.Gread=format.Gread;return finish();}
    if(!meter.consume({1,0,0,0,0,0,0,0,0})){out.状态=材料状态_B1::数量预算不足;return finish();}
    const auto source=读取任一当前事实(l1_,r.来源);
    out.Gread=source.读取事实代次;
    if(source.读取事实代次!=r.G0){out.状态=材料状态_B1::事实代次漂移;return finish();}
    if(source.状态!=L1所有者范围读取状态::成功||!source.事实){out.状态=map_read(source.状态);return finish();}
    const auto*sourceNode=std::get_if<L1所有者范围节点事实>(&*source.事实);
    if(!sourceNode){out.状态=材料状态_B1::内部不一致;return finish();}
    if(!live_at(*sourceNode,r.G0)){out.状态=材料状态_B1::内部不一致;return finish();}

    const auto family=l1_.读取所有者范围当前源关系组({L1所有者范围CRUD合同版本,layout_.族锚点,layout_.族成员});
    out.Gread=family.读取事实代次;
    if(family.读取事实代次!=r.G0){out.状态=材料状态_B1::事实代次漂移;return finish();}
    if(family.状态!=L1所有者范围读取状态::成功){out.状态=map_read(family.状态);return finish();}
    const auto count=family.关系组.size();
    if(!meter.consume({0,count,0,0,0,count,0,0,0})){out.状态=材料状态_B1::数量预算不足;return finish();}
    std::optional<材料事实_B1> same;
    for(const auto&e:family.关系组){
      if(meter.left.最大节点数<2||meter.left.最大关系数<3||meter.left.最大值数<3||meter.left.最大值元素数<32){out.状态=材料状态_B1::数量预算不足;return finish();}
      const 材料读取请求_B1 qr{1,r.G0,{e.目标节点},meter.call_budget()};
      const auto rr=读取材料(qr);out.Gread=rr.Gread;
      if(!meter.consume(rr.用量)){out.状态=材料状态_B1::数量预算不足;return finish();}
      if(!rr.成功(qr)){out.状态=rr.状态;return finish();}
      if(rr.材料->格式==r.格式&&rr.材料->SHA256==digest){
        if(rr.材料->完整载荷!=r.完整载荷||same){out.状态=材料状态_B1::内部不一致;return finish();}
        same=rr.材料;
      }
    }
    out.用量=meter.used;
    if(same){out.状态=材料状态_B1::已复用;out.当前材料=std::move(same);out.当前终态=当前终态_B1::原后态仍成立;out.发布=材料发布状态_B1::确认未发布;return finish();}

    const auto final=material_path(directory_,r.格式,digest);
    if(std::filesystem::exists(final)){
      if(r.完整载荷.size()>meter.left.最大材料字节数){out.状态=材料状态_B1::数量预算不足;return finish();}
      if(read_file(directory_,final,meter.left.最大材料字节数)!=r.完整载荷){out.状态=材料状态_B1::内部不一致;return finish();}
      (void)meter.consume({0,0,0,0,0,0,0,r.完整载荷.size(),0});
    }else{
      const auto temp=directory_/(std::to_string(r.幂等身份.值)+".candidate");
      HANDLE handle=CreateFileW(temp.c_str(),GENERIC_WRITE|FILE_READ_ATTRIBUTES,0,nullptr,CREATE_ALWAYS,
                                FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OPEN_REPARSE_POINT,nullptr);
      if(handle==INVALID_HANDLE_VALUE)throw std::runtime_error("file");
      bool pathOk=true;try{require_material_file(handle,directory_);}catch(...){pathOk=false;}
      DWORD written{};const bool ok=pathOk&&WriteFile(handle,r.完整载荷.data(),static_cast<DWORD>(r.完整载荷.size()),&written,nullptr)&&written==r.完整载荷.size()&&FlushFileBuffers(handle);
      CloseHandle(handle);if(!ok)throw std::runtime_error("file");
      if(r.完整载荷.size()>meter.left.最大材料字节数){out.状态=材料状态_B1::数量预算不足;return finish();}
      if(read_file(directory_,temp,meter.left.最大材料字节数)!=r.完整载荷)throw std::runtime_error("file");
      (void)meter.consume({0,0,0,0,0,0,0,r.完整载荷.size(),0});
      if(!MoveFileExW(temp.c_str(),final.c_str(),MOVEFILE_WRITE_THROUGH)){
        if(!std::filesystem::exists(final)||r.完整载荷.size()>meter.left.最大材料字节数||read_file(directory_,final,meter.left.最大材料字节数)!=r.完整载荷)throw std::runtime_error("file");
        (void)meter.consume({0,0,0,0,0,0,0,r.完整载荷.size(),0});
        std::filesystem::remove(temp);
      }
    }
    entered=true;out.发布=材料发布状态_B1::可能发布;
    const auto saved=port_.提交所有者范围中性写集(expected);out.Gread=saved.事实代次;
    if(saved.状态!=L1所有者范围写入状态::成功){out.状态=map_write(saved.状态);return finish();}
    out.首次H=saved.事实代次;out.发布=材料发布状态_B1::确认发布;稳定编码 mid{};
    for(auto[k,id]:saved.新编码映射)if(k.值==1)mid=id;
    if(meter.left.最大节点数<2||meter.left.最大关系数<3||meter.left.最大值数<3||meter.left.最大值元素数<32){out.状态=材料状态_B1::数量预算不足;return finish();}
    const 材料读取请求_B1 rrq{1,saved.事实代次,{mid},meter.call_budget()};
    const auto rr=读取材料(rrq);out.Gread=rr.Gread;
    if(!meter.consume(rr.用量)){out.状态=材料状态_B1::数量预算不足;return finish();}
    if(!rr.成功(rrq)){out.状态=rr.状态;return finish();}
    out.用量=meter.used;
    out.首次材料=out.当前材料=rr.材料;out.当前终态=当前终态_B1::原后态仍成立;
    out.本次来源已记录=true;out.状态=材料状态_B1::已创建;out.发布=材料发布状态_B1::确认发布;return finish();
  }catch(const std::bad_alloc&){out.状态=entered ? 材料状态_B1::可能已发布 : 材料状态_B1::资源失败;}
  catch(const std::length_error&){out.状态=entered ? 材料状态_B1::可能已发布 : 材料状态_B1::数量预算不足;}
  catch(...){out.状态=entered ? 材料状态_B1::可能已发布 : 材料状态_B1::资源失败;}
  if(entered)out.发布=材料发布状态_B1::可能发布;return finish();
}

材料退出结果_B1 不可变材料数据服务::退出材料(const 材料退出请求_B1&r)noexcept{
  材料退出结果_B1 out;bool entered=false;
  material_meter meter(r.预算);
  material_usage_guard usage{out.用量,meter};
  auto finish=[&]() -> 材料退出结果_B1 { usage.sync(); return std::move(out); };
  try{
    out.原请求=r;out.Gread=r.G0;std::scoped_lock lock(mutex_);
    if(r.版本!=1||!r.G0||r.G0==UINT64_MAX||!有效(r.幂等身份)||!有效(r.材料)||!material_budget(r.预算))return finish();
    const auto first=port_.读取首次写入材料({L1所有者范围首次写入读取合同版本,r.幂等身份});
    out.Gread=first.读取事实代次;
    const bool replay=first.状态==L1所有者范围读取状态::成功;
    if(replay){
      if(!first.首次规范化写集||!first.首次写入结果){out.状态=材料状态_B1::可能已发布;out.发布=材料发布状态_B1::可能发布;return finish();}
      out.首次H=first.首次写入结果->事实代次;
      const auto& ws=*first.首次规范化写集;
      const auto& saved=*first.首次写入结果;
      if(ws.合同版本!=L1所有者范围CRUD合同版本||ws.期望事实代次!=r.G0||
         ws.写入幂等身份!=r.幂等身份||!ws.节点.empty()||!ws.关系.empty()||
         !ws.值.empty()||!ws.属性槽变更.empty()||ws.退出事实.size()!=6||
         !std::ranges::is_sorted(ws.退出事实,std::ranges::less{},&稳定编码::值)||
         std::adjacent_find(ws.退出事实.begin(),ws.退出事实.end(),
             [](稳定编码 a,稳定编码 b){return a.值==b.值;})!=ws.退出事实.end()||
         !std::ranges::binary_search(ws.退出事实,r.材料.值.值,
             std::ranges::less{},&稳定编码::值)||!*out.首次H||
         *out.首次H!=r.G0+1||saved.状态!=L1所有者范围写入状态::成功||
         saved.所有者!=port_.所有者身份()||saved.写入幂等身份!=r.幂等身份||
         saved.事实代次!=*out.首次H||!saved.是否形成内存权威发布){
        out.状态=材料状态_B1::幂等冲突;return finish();
      }
    }else if(first.状态==L1所有者范围读取状态::未找到){
      if(first.读取事实代次!=r.G0){out.状态=材料状态_B1::事实代次漂移;return finish();}
    }else{out.状态=map_read(first.状态);return finish();}
    if(replay){
      const 材料读取请求_B1 currentRequest{1,first.读取事实代次,r.材料,meter.call_budget()};
      const auto current=读取材料(currentRequest);out.Gread=current.Gread;
      if(!meter.consume(current.用量)){out.状态=材料状态_B1::数量预算不足;return finish();}
      if(current.状态!=材料状态_B1::未找到||current.材料){
        out.状态=current.状态==材料状态_B1::资源失败 ? 材料状态_B1::资源失败 : 材料状态_B1::内部不一致;
        return finish();
      }
      out.发布=材料发布状态_B1::确认发布;
      out.当前终态=当前终态_B1::已删除;
      out.状态=材料状态_B1::精确重复;
      return finish();
    }

    const 材料读取请求_B1 beforeRequest{1,r.G0,r.材料,meter.call_budget()};
    const auto before=读取材料(beforeRequest);out.Gread=before.Gread;
    if(!meter.consume(before.用量)){out.状态=材料状态_B1::数量预算不足;return finish();}
    if(!before.成功(beforeRequest)){out.状态=before.状态;return finish();}
    auto expected=material_exit_set(r,*before.材料);
    const L1节点当前完整引用读取请求_v2 qr{L1节点当前完整引用读取合同版本,r.材料.值,r.G0};
    const auto refs=l1_.读取节点全部当前引用_v2(qr);out.Gread=refs.读取事实代次;
    if(refs.状态!=L1节点当前完整引用读取状态_v2::成功
        ||refs.合同版本!=qr.合同版本||refs.节点!=qr.节点
        ||refs.期望事实代次!=qr.期望事实代次||refs.读取事实代次!=qr.期望事实代次){
      out.状态=refs.状态==L1节点当前完整引用读取状态_v2::事实代次漂移 ? 材料状态_B1::事实代次漂移 :
        refs.状态==L1节点当前完整引用读取状态_v2::资源失败 ? 材料状态_B1::资源失败 : 材料状态_B1::内部不一致;
      return finish();
    }
    世界结构用量_B1 referenceUsage{};
    for(const auto& x:refs.引用){
      if(std::holds_alternative<L1所有者范围关系事实>(x))++referenceUsage.最大关系数;
      else ++referenceUsage.最大值数;
    }
    if(!meter.consume(referenceUsage)){out.状态=材料状态_B1::数量预算不足;return finish();}
    for(const auto&x:refs.引用){const auto id=std::visit([](const auto&f){return f.编码;},x);
      if(std::find(expected.退出事实.begin(),expected.退出事实.end(),id)==expected.退出事实.end()){
        out.状态=材料状态_B1::引用保护;return finish();
      }
    }
    entered=true;out.发布=材料发布状态_B1::可能发布;
    const auto saved=port_.提交所有者范围中性写集(expected);out.Gread=saved.事实代次;
    if(saved.状态!=L1所有者范围写入状态::成功){
      out.状态=map_write(saved.状态);return finish();
    }
    out.首次H=saved.事实代次;out.发布=材料发布状态_B1::确认发布;
    if(!*out.首次H||*out.首次H!=r.G0+1){out.状态=材料状态_B1::内部不一致;return finish();}
    const 材料读取请求_B1 verifyRequest{1,saved.事实代次,r.材料,meter.call_budget()};
    const auto verify=读取材料(verifyRequest);out.Gread=verify.Gread;
    if(!meter.consume(verify.用量)){out.状态=材料状态_B1::数量预算不足;return finish();}
    if(verify.状态!=材料状态_B1::未找到||verify.材料){out.状态=材料状态_B1::内部不一致;return finish();}
    out.当前终态=当前终态_B1::已删除;
    out.状态=材料状态_B1::已删除;return finish();
  }catch(const std::bad_alloc&){out.状态=out.发布==材料发布状态_B1::确认发布 ? 材料状态_B1::资源失败 : entered ? 材料状态_B1::可能已发布 : 材料状态_B1::资源失败;}
  catch(...){out.状态=out.发布==材料发布状态_B1::确认发布 ? 材料状态_B1::内部不一致 : entered ? 材料状态_B1::可能已发布 : 材料状态_B1::内部不一致;}
  if(entered&&out.发布!=材料发布状态_B1::确认发布)out.发布=材料发布状态_B1::可能发布;return finish();
}


} // namespace 海中鱼巣
