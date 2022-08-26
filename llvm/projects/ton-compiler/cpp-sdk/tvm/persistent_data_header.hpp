#pragma once

namespace tvm {

template<typename T>
struct is_resumable : std::false_type {};
template<typename T>
struct is_resumable<schema::resumable<T>> : std::true_type {};
template<typename T>
constexpr bool is_resumable_v = is_resumable<T>::value;

template<typename T>
struct mem_fn_ret {};
template<typename Rv, typename T, typename... Args>
struct mem_fn_ret<Rv (T::*)(Args...)> {
  using type = Rv;
};

template<auto Func>
constexpr bool is_coroutine_v = is_resumable_v<typename mem_fn_ret<decltype(Func)>::type>;

template<class Interface, unsigned Index, unsigned RestMethods>
struct interface_has_coroutines_impl {
  using RetType = get_interface_method_rv<Interface, Index>;
  static constexpr bool value = is_resumable_v<RetType> ||
    interface_has_coroutines_impl<Interface, Index + 1, RestMethods - 1>::value;
};
template<class Interface, unsigned Index>
struct interface_has_coroutines_impl<Interface, Index, 0> : std::false_type {};
template<class Interface>
constexpr bool interface_has_coroutines_v =
  interface_has_coroutines_impl<Interface, 0,
    get_interface_methods_count<Interface>::value>::value;
    
template<class ReplayAttackProtection, bool has_replay, bool has_awaiting>
constexpr auto get_persistent_header_tuple() {
  if constexpr (has_replay && has_awaiting)
    return std::make_tuple(typename ReplayAttackProtection::persistent_t{}, awaiting_responses_map{});
  else if constexpr (has_replay && !has_awaiting)
    return typename ReplayAttackProtection::persistent_t{};
  else if constexpr (!has_replay && has_awaiting)
    return awaiting_responses_map{};
  else
    return std::tuple<>();
}

template<class Interface, class ReplayAttackProtection>
struct persistent_header_info {
  static constexpr bool has_replay = !std::is_void_v<ReplayAttackProtection>;
  static constexpr bool has_awaiting = interface_has_coroutines_v<Interface>;

  using common_hdr = decltype(get_persistent_header_tuple<ReplayAttackProtection, has_replay, has_awaiting>());

  static constexpr bool non_empty = has_replay || has_awaiting;
};

template<class Interface, class ReplayAttackProtection>
using persistent_data_header_t = typename persistent_header_info<Interface, ReplayAttackProtection>::common_hdr;

template<class Interface, class ReplayAttackProtection>
struct persistent_data_header {
  using info = persistent_header_info<Interface, ReplayAttackProtection>;
  using hdr_t = typename info::common_hdr;
  static __always_inline auto& get_replay_data(hdr_t &hdr) {
    if constexpr (info::has_replay && info::has_awaiting)
      return std::get<0>(hdr);
    else
      return hdr;
  }
  static __always_inline auto& get_awaiting(hdr_t &hdr) {
    if constexpr (info::has_replay && info::has_awaiting)
      return std::get<1>(hdr);
    else
      return hdr;
  }
  static __always_inline hdr_t init() {
    if constexpr (info::has_replay && info::has_awaiting)
      return { ReplayAttackProtection::init(), {} };
    else
      return {};
  }
};

} // namespace tvm

