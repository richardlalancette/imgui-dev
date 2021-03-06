﻿# pragma once
# include "imguiex_internal.h"

namespace ImGuiEx {

struct CanvasView
{
    ImVec2 Origin        = ImVec2(0.0f, 0.0f);
    ImVec2 RoundedOrigin = ImVec2(0.0f, 0.0f);
    float  Scale         = 1.0f;
    float  InvScale      = 1.0f;

    void Set(const ImVec2& origin, float scale)
    {
        SetOrigin(origin);
        SetScale(scale);
    }

    void SetOrigin(const ImVec2& origin)
    {
        Origin        = origin;
        RoundedOrigin = ImFloor(origin);
    }

    void SetScale(float scale)
    {
        Scale    = scale;
        InvScale = scale ? 1.0f / scale : 0.0f;
    }

    ImVec2 ToWorld(const ImVec2& point) const { return point * Scale + RoundedOrigin;      }
    ImVec2 ToLocal(const ImVec2& point) const { return (point - RoundedOrigin) * InvScale; }
    ImRect ToWorld(const ImRect& rect) const { return ImRect(ToWorld(rect.Min), ToWorld(rect.Max)); }
    ImRect ToLocal(const ImRect& rect) const { return ImRect(ToLocal(rect.Min), ToLocal(rect.Max)); }
};

struct Canvas
{
    Canvas(ImGuiID id);

    ImGuiID GetID() const { return m_ID; }

    // Returns false if canvas is not visible.
    bool Begin(Canvas* parent, const ImVec2& size = ImVec2(0, 0));
    void End();

    // Sets viewport over canvas:
    //   'worldOrigin' - position from top left widget corner where center
    //                   of virtual coordinate system is placed
    //   'scale'       - how much content of viewport is scaled
    void SetView(const ImVec2& worldOrigin, float scale = 1.0f);
    void SetView(const CanvasView& view);

    // Center current viewport over 'virtualPoint'.
    void CenterView(const ImVec2& virtualPoint);

    // Center viewport over 'virtualRect'.
    void CenterView(const ImRect& virtualRect);

    void Suspend();
    void Resume();

    ImVec2 ToParent(const ImVec2& point) const;
    ImVec2 FromParent(const ImVec2& point) const;

    ImVec2 ToWorld(const ImVec2& point) const;
    ImVec2 FromWorld(const ImVec2& point) const;

    const ImRect&     ContentRect() const { return m_ContentRect; }
    const CanvasView& ContentView() const { return m_ContentView; }
    const CanvasView& View()        const { return m_View; }
    const ImRect&     ViewRect()    const { return m_ViewRect; }
    const ImVec2&     ViewOrigin()  const { return m_View.Origin; }
    float             ViewScale()   const { return m_View.Scale; }
    bool              IsSuspended() const { return m_SuspendCounter > 0; }

private:
    void SaveInputState();
    void RestoreInputState();

    void EnterLocalSpace();
    void LeaveLocalSpace();

    Canvas* m_Parent = nullptr;

    ImGuiID m_ID;
    ImVec2 m_Size; // Size as user passed in
    ImVec2 m_StartPos;
    ImVec2 m_CurrentSize;
    ImRect m_ContentRect;

    ImDrawList* m_DrawList = nullptr;
    int m_DrawListCommadBufferSize;
    int m_DrawListStartVertexIndex;

    CanvasView m_ContentView;
    CanvasView m_View;
    ImRect     m_ViewRect;

    int m_SuspendCounter = 0;

    ImVec2 m_MousePosBackup;
    ImVec2 m_MousePosPrevBackup;
    ImVec2 m_MouseClickedPosBackup[IM_ARRAYSIZE(ImGuiIO::MouseClickedPos)];
    ImVec2 m_WindowCursorMaxBackup;
};

} // namespace ImGuiExtras
