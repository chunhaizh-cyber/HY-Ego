SET NOCOUNT ON;

IF DB_ID(N'FishNestRuntime') IS NULL
BEGIN
    CREATE DATABASE [FishNestRuntime];
END
GO

USE [FishNestRuntime];
GO

IF OBJECT_ID(N'dbo.latest_runtime_snapshot', N'U') IS NULL
BEGIN
    CREATE TABLE dbo.latest_runtime_snapshot
    (
        snapshot_key              NVARCHAR(64)   NOT NULL CONSTRAINT PK_latest_runtime_snapshot PRIMARY KEY,
        captured_at               DATETIME2(3)   NOT NULL,
        source_host               NVARCHAR(128)  NULL,
        source_process_id         INT            NULL,
        backend_online            BIT            NOT NULL CONSTRAINT DF_latest_runtime_snapshot_backend_online DEFAULT (0),
        host_running              BIT            NOT NULL CONSTRAINT DF_latest_runtime_snapshot_host_running DEFAULT (0),
        stop_requested            BIT            NOT NULL CONSTRAINT DF_latest_runtime_snapshot_stop_requested DEFAULT (0),
        self_running              BIT            NOT NULL CONSTRAINT DF_latest_runtime_snapshot_self_running DEFAULT (0),
        initialized               BIT            NOT NULL CONSTRAINT DF_latest_runtime_snapshot_initialized DEFAULT (0),
        healthy                   BIT            NOT NULL CONSTRAINT DF_latest_runtime_snapshot_healthy DEFAULT (0),
        restart_requested         BIT            NOT NULL CONSTRAINT DF_latest_runtime_snapshot_restart_requested DEFAULT (0),
        fatal_count               INT            NOT NULL CONSTRAINT DF_latest_runtime_snapshot_fatal_count DEFAULT (0),
        restart_count             INT            NOT NULL CONSTRAINT DF_latest_runtime_snapshot_restart_count DEFAULT (0),
        camera_enabled            BIT            NOT NULL CONSTRAINT DF_latest_runtime_snapshot_camera_enabled DEFAULT (0),
        camera_running            BIT            NOT NULL CONSTRAINT DF_latest_runtime_snapshot_camera_running DEFAULT (0),
        camera_virtual            BIT            NOT NULL CONSTRAINT DF_latest_runtime_snapshot_camera_virtual DEFAULT (0),
        camera_frames             BIGINT         NOT NULL CONSTRAINT DF_latest_runtime_snapshot_camera_frames DEFAULT (0),
        camera_candidates         BIGINT         NOT NULL CONSTRAINT DF_latest_runtime_snapshot_camera_candidates DEFAULT (0),
        world_count               BIGINT         NOT NULL CONSTRAINT DF_latest_runtime_snapshot_world_count DEFAULT (0),
        need_count                BIGINT         NOT NULL CONSTRAINT DF_latest_runtime_snapshot_need_count DEFAULT (0),
        task_count                BIGINT         NOT NULL CONSTRAINT DF_latest_runtime_snapshot_task_count DEFAULT (0),
        safe_task_count           BIGINT         NOT NULL CONSTRAINT DF_latest_runtime_snapshot_safe_task_count DEFAULT (0),
        service_task_count        BIGINT         NOT NULL CONSTRAINT DF_latest_runtime_snapshot_service_task_count DEFAULT (0),
        leaf_task_count           BIGINT         NOT NULL CONSTRAINT DF_latest_runtime_snapshot_leaf_task_count DEFAULT (0),
        method_count              BIGINT         NOT NULL CONSTRAINT DF_latest_runtime_snapshot_method_count DEFAULT (0),
        state_count               BIGINT         NOT NULL CONSTRAINT DF_latest_runtime_snapshot_state_count DEFAULT (0),
        dynamic_count             BIGINT         NOT NULL CONSTRAINT DF_latest_runtime_snapshot_dynamic_count DEFAULT (0),
        event_dynamic_count       BIGINT         NOT NULL CONSTRAINT DF_latest_runtime_snapshot_event_dynamic_count DEFAULT (0),
        causality_count           BIGINT         NOT NULL CONSTRAINT DF_latest_runtime_snapshot_causality_count DEFAULT (0),
        has_self_safety           BIT            NOT NULL CONSTRAINT DF_latest_runtime_snapshot_has_self_safety DEFAULT (0),
        self_safety_value         BIGINT         NOT NULL CONSTRAINT DF_latest_runtime_snapshot_self_safety_value DEFAULT (0),
        has_self_service          BIT            NOT NULL CONSTRAINT DF_latest_runtime_snapshot_has_self_service DEFAULT (0),
        self_service_value        BIGINT         NOT NULL CONSTRAINT DF_latest_runtime_snapshot_self_service_value DEFAULT (0),
        snapshot_utc_ms           BIGINT         NOT NULL CONSTRAINT DF_latest_runtime_snapshot_snapshot_utc_ms DEFAULT (0),
        last_input_seq            BIGINT         NOT NULL CONSTRAINT DF_latest_runtime_snapshot_last_input_seq DEFAULT (0),
        top_need_weight           BIGINT         NOT NULL CONSTRAINT DF_latest_runtime_snapshot_top_need_weight DEFAULT (0),
        top_need_satisfied        BIT            NOT NULL CONSTRAINT DF_latest_runtime_snapshot_top_need_satisfied DEFAULT (0),
        top_need_type             NVARCHAR(256)  NULL,
        last_input_text           NVARCHAR(MAX)  NULL,
        last_command              NVARCHAR(1024) NULL,
        recent_fault              NVARCHAR(1024) NULL,
        host_fault                NVARCHAR(1024) NULL,
        camera_status             NVARCHAR(1024) NULL
    );
END
GO

IF COL_LENGTH(N'dbo.latest_runtime_snapshot', N'causality_count') IS NULL
BEGIN
    ALTER TABLE dbo.latest_runtime_snapshot
        ADD causality_count BIGINT NOT NULL
            CONSTRAINT DF_latest_runtime_snapshot_causality_count DEFAULT (0);
END
GO

IF COL_LENGTH(N'dbo.latest_runtime_snapshot', N'has_self_safety') IS NULL
BEGIN
    ALTER TABLE dbo.latest_runtime_snapshot
        ADD has_self_safety BIT NOT NULL
            CONSTRAINT DF_latest_runtime_snapshot_has_self_safety DEFAULT (0);
END
GO

IF COL_LENGTH(N'dbo.latest_runtime_snapshot', N'self_safety_value') IS NULL
BEGIN
    ALTER TABLE dbo.latest_runtime_snapshot
        ADD self_safety_value BIGINT NOT NULL
            CONSTRAINT DF_latest_runtime_snapshot_self_safety_value DEFAULT (0);
END
GO

IF COL_LENGTH(N'dbo.latest_runtime_snapshot', N'has_self_service') IS NULL
BEGIN
    ALTER TABLE dbo.latest_runtime_snapshot
        ADD has_self_service BIT NOT NULL
            CONSTRAINT DF_latest_runtime_snapshot_has_self_service DEFAULT (0);
END
GO

IF COL_LENGTH(N'dbo.latest_runtime_snapshot', N'self_service_value') IS NULL
BEGIN
    ALTER TABLE dbo.latest_runtime_snapshot
        ADD self_service_value BIGINT NOT NULL
            CONSTRAINT DF_latest_runtime_snapshot_self_service_value DEFAULT (0);
END
GO

IF COL_LENGTH(N'dbo.latest_runtime_snapshot', N'snapshot_utc_ms') IS NULL
BEGIN
    ALTER TABLE dbo.latest_runtime_snapshot
        ADD snapshot_utc_ms BIGINT NOT NULL
            CONSTRAINT DF_latest_runtime_snapshot_snapshot_utc_ms DEFAULT (0);
END
GO

IF OBJECT_ID(N'dbo.task_event', N'U') IS NULL
BEGIN
    CREATE TABLE dbo.task_event
    (
        id                        BIGINT         NOT NULL IDENTITY(1,1) CONSTRAINT PK_task_event PRIMARY KEY,
        event_time                DATETIME2(3)   NOT NULL,
        task_key                  NVARCHAR(128)  NOT NULL,
        parent_task_key           NVARCHAR(128)  NULL,
        task_type                 NVARCHAR(128)  NULL,
        event_type                NVARCHAR(64)   NOT NULL,
        task_status               NVARCHAR(64)   NULL,
        return_message_type       NVARCHAR(64)   NULL,
        need_key                  NVARCHAR(128)  NULL,
        method_key                NVARCHAR(128)  NULL,
        summary_text              NVARCHAR(2048) NULL,
        detail_json               NVARCHAR(MAX)  NULL,
        source_host               NVARCHAR(128)  NULL,
        source_process_id         INT            NULL
    );

    CREATE INDEX IX_task_event_event_time ON dbo.task_event(event_time DESC);
    CREATE INDEX IX_task_event_task_key ON dbo.task_event(task_key, event_time DESC);
END
GO

IF OBJECT_ID(N'dbo.need_event', N'U') IS NULL
BEGIN
    CREATE TABLE dbo.need_event
    (
        id                        BIGINT         NOT NULL IDENTITY(1,1) CONSTRAINT PK_need_event PRIMARY KEY,
        event_time                DATETIME2(3)   NOT NULL,
        need_key                  NVARCHAR(128)  NOT NULL,
        root_need_key             NVARCHAR(128)  NULL,
        need_type                 NVARCHAR(128)  NULL,
        event_type                NVARCHAR(64)   NOT NULL,
        run_state                 NVARCHAR(64)   NULL,
        weight_value              BIGINT         NULL,
        is_satisfied              BIT            NULL,
        current_value_text        NVARCHAR(256)  NULL,
        target_value_text         NVARCHAR(256)  NULL,
        summary_text              NVARCHAR(2048) NULL,
        detail_json               NVARCHAR(MAX)  NULL,
        source_host               NVARCHAR(128)  NULL,
        source_process_id         INT            NULL
    );

    CREATE INDEX IX_need_event_event_time ON dbo.need_event(event_time DESC);
    CREATE INDEX IX_need_event_need_key ON dbo.need_event(need_key, event_time DESC);
END
GO

IF OBJECT_ID(N'dbo.causality_event', N'U') IS NULL
BEGIN
    CREATE TABLE dbo.causality_event
    (
        id                        BIGINT         NOT NULL IDENTITY(1,1) CONSTRAINT PK_causality_event PRIMARY KEY,
        event_time                DATETIME2(3)   NOT NULL,
        causality_key             NVARCHAR(128)  NOT NULL,
        action_key                NVARCHAR(128)  NULL,
        anchor_dynamic_key        NVARCHAR(128)  NULL,
        observation_count         BIGINT         NOT NULL CONSTRAINT DF_causality_event_observation_count DEFAULT (0),
        stability_q               BIGINT         NOT NULL CONSTRAINT DF_causality_event_stability_q DEFAULT (0),
        condition_summary         NVARCHAR(512)  NULL,
        process_summary           NVARCHAR(512)  NULL,
        result_summary            NVARCHAR(512)  NULL,
        condition_signature       BIGINT         NULL,
        process_signature         BIGINT         NULL,
        result_signature          BIGINT         NULL,
        detail_json               NVARCHAR(MAX)  NULL,
        source_host               NVARCHAR(128)  NULL,
        source_process_id         INT            NULL
    );

    CREATE INDEX IX_causality_event_event_time ON dbo.causality_event(event_time DESC);
    CREATE INDEX IX_causality_event_causality_key ON dbo.causality_event(causality_key, event_time DESC);
END
GO

CREATE OR ALTER VIEW dbo.v_runtime_overview
AS
SELECT
    snapshot_key,
    captured_at,
    source_host,
    source_process_id,
    backend_online,
    host_running,
    self_running,
    initialized,
    healthy,
    need_count,
    task_count,
    safe_task_count,
    service_task_count,
    leaf_task_count,
    method_count,
    state_count,
    dynamic_count,
    event_dynamic_count,
    causality_count,
    has_self_safety,
    self_safety_value,
    has_self_service,
    self_service_value,
    snapshot_utc_ms,
    top_need_type,
    top_need_weight,
    top_need_satisfied,
    recent_fault,
    host_fault,
    camera_status
FROM dbo.latest_runtime_snapshot;
GO

CREATE OR ALTER PROCEDURE dbo.upsert_latest_runtime_snapshot
    @snapshot_key            NVARCHAR(64),
    @captured_at             DATETIME2(3),
    @source_host             NVARCHAR(128) = NULL,
    @source_process_id       INT = NULL,
    @backend_online          BIT = 0,
    @host_running            BIT = 0,
    @stop_requested          BIT = 0,
    @self_running            BIT = 0,
    @initialized             BIT = 0,
    @healthy                 BIT = 0,
    @restart_requested       BIT = 0,
    @fatal_count             INT = 0,
    @restart_count           INT = 0,
    @camera_enabled          BIT = 0,
    @camera_running          BIT = 0,
    @camera_virtual          BIT = 0,
    @camera_frames           BIGINT = 0,
    @camera_candidates       BIGINT = 0,
    @world_count             BIGINT = 0,
    @need_count              BIGINT = 0,
    @task_count              BIGINT = 0,
    @safe_task_count         BIGINT = 0,
    @service_task_count      BIGINT = 0,
    @leaf_task_count         BIGINT = 0,
    @method_count            BIGINT = 0,
    @state_count             BIGINT = 0,
    @dynamic_count           BIGINT = 0,
    @event_dynamic_count     BIGINT = 0,
    @causality_count         BIGINT = 0,
    @has_self_safety         BIT = 0,
    @self_safety_value       BIGINT = 0,
    @has_self_service        BIT = 0,
    @self_service_value      BIGINT = 0,
    @snapshot_utc_ms         BIGINT = 0,
    @last_input_seq          BIGINT = 0,
    @top_need_weight         BIGINT = 0,
    @top_need_satisfied      BIT = 0,
    @top_need_type           NVARCHAR(256) = NULL,
    @last_input_text         NVARCHAR(MAX) = NULL,
    @last_command            NVARCHAR(1024) = NULL,
    @recent_fault            NVARCHAR(1024) = NULL,
    @host_fault              NVARCHAR(1024) = NULL,
    @camera_status           NVARCHAR(1024) = NULL
AS
BEGIN
    SET NOCOUNT ON;

    MERGE dbo.latest_runtime_snapshot AS target
    USING (
        SELECT
            @snapshot_key AS snapshot_key
    ) AS source
    ON target.snapshot_key = source.snapshot_key
    WHEN MATCHED THEN
        UPDATE SET
            captured_at = @captured_at,
            source_host = @source_host,
            source_process_id = @source_process_id,
            backend_online = @backend_online,
            host_running = @host_running,
            stop_requested = @stop_requested,
            self_running = @self_running,
            initialized = @initialized,
            healthy = @healthy,
            restart_requested = @restart_requested,
            fatal_count = @fatal_count,
            restart_count = @restart_count,
            camera_enabled = @camera_enabled,
            camera_running = @camera_running,
            camera_virtual = @camera_virtual,
            camera_frames = @camera_frames,
            camera_candidates = @camera_candidates,
            world_count = @world_count,
            need_count = @need_count,
            task_count = @task_count,
            safe_task_count = @safe_task_count,
            service_task_count = @service_task_count,
            leaf_task_count = @leaf_task_count,
            method_count = @method_count,
            state_count = @state_count,
            dynamic_count = @dynamic_count,
            event_dynamic_count = @event_dynamic_count,
            causality_count = @causality_count,
            has_self_safety = @has_self_safety,
            self_safety_value = @self_safety_value,
            has_self_service = @has_self_service,
            self_service_value = @self_service_value,
            snapshot_utc_ms = @snapshot_utc_ms,
            last_input_seq = @last_input_seq,
            top_need_weight = @top_need_weight,
            top_need_satisfied = @top_need_satisfied,
            top_need_type = @top_need_type,
            last_input_text = @last_input_text,
            last_command = @last_command,
            recent_fault = @recent_fault,
            host_fault = @host_fault,
            camera_status = @camera_status
    WHEN NOT MATCHED THEN
        INSERT
        (
            snapshot_key,
            captured_at,
            source_host,
            source_process_id,
            backend_online,
            host_running,
            stop_requested,
            self_running,
            initialized,
            healthy,
            restart_requested,
            fatal_count,
            restart_count,
            camera_enabled,
            camera_running,
            camera_virtual,
            camera_frames,
            camera_candidates,
            world_count,
            need_count,
            task_count,
            safe_task_count,
            service_task_count,
            leaf_task_count,
            method_count,
            state_count,
            dynamic_count,
            event_dynamic_count,
            causality_count,
            has_self_safety,
            self_safety_value,
            has_self_service,
            self_service_value,
            snapshot_utc_ms,
            last_input_seq,
            top_need_weight,
            top_need_satisfied,
            top_need_type,
            last_input_text,
            last_command,
            recent_fault,
            host_fault,
            camera_status
        )
        VALUES
        (
            @snapshot_key,
            @captured_at,
            @source_host,
            @source_process_id,
            @backend_online,
            @host_running,
            @stop_requested,
            @self_running,
            @initialized,
            @healthy,
            @restart_requested,
            @fatal_count,
            @restart_count,
            @camera_enabled,
            @camera_running,
            @camera_virtual,
            @camera_frames,
            @camera_candidates,
            @world_count,
            @need_count,
            @task_count,
            @safe_task_count,
            @service_task_count,
            @leaf_task_count,
            @method_count,
            @state_count,
            @dynamic_count,
            @event_dynamic_count,
            @causality_count,
            @has_self_safety,
            @self_safety_value,
            @has_self_service,
            @self_service_value,
            @snapshot_utc_ms,
            @last_input_seq,
            @top_need_weight,
            @top_need_satisfied,
            @top_need_type,
            @last_input_text,
            @last_command,
            @recent_fault,
            @host_fault,
            @camera_status
        );
END
GO
