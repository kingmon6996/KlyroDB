#ifndef KLYRO_WAL_RECOVERY_STATE_HPP
#define KLYRO_WAL_RECOVERY_STATE_HPP

namespace klyro::wal {

enum class RecoveryState {
    NotRequired,
    Required,
    Analysis,
    Redo,
    Undo,
    Complete,
    Failed
};

} // namespace klyro::wal

#endif // KLYRO_WAL_RECOVERY_STATE_HPP
