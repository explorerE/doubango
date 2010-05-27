/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.39
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */


using System;
using System.Runtime.InteropServices;

public class SipCallback : IDisposable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal SipCallback(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  internal static HandleRef getCPtr(SipCallback obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~SipCallback() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if(swigCPtr.Handle != IntPtr.Zero && swigCMemOwn) {
        swigCMemOwn = false;
        tinyWRAPPINVOKE.delete_SipCallback(swigCPtr);
      }
      swigCPtr = new HandleRef(null, IntPtr.Zero);
      GC.SuppressFinalize(this);
    }
  }

  public SipCallback() : this(tinyWRAPPINVOKE.new_SipCallback(), true) {
    SwigDirectorConnect();
  }

  public virtual int OnRegistrationChanged(RegistrationEvent e) {
    int ret = ((this.GetType() == typeof(SipCallback)) ? tinyWRAPPINVOKE.SipCallback_OnRegistrationChanged(swigCPtr, RegistrationEvent.getCPtr(e)) : tinyWRAPPINVOKE.SipCallback_OnRegistrationChangedSwigExplicitSipCallback(swigCPtr, RegistrationEvent.getCPtr(e)));
    return ret;
  }

  public virtual int OnSubscriptionChanged(SubscriptionEvent e) {
    int ret = ((this.GetType() == typeof(SipCallback)) ? tinyWRAPPINVOKE.SipCallback_OnSubscriptionChanged(swigCPtr, SubscriptionEvent.getCPtr(e)) : tinyWRAPPINVOKE.SipCallback_OnSubscriptionChangedSwigExplicitSipCallback(swigCPtr, SubscriptionEvent.getCPtr(e)));
    return ret;
  }

  public virtual int OnPublicationChanged(PublicationEvent e) {
    int ret = ((this.GetType() == typeof(SipCallback)) ? tinyWRAPPINVOKE.SipCallback_OnPublicationChanged(swigCPtr, PublicationEvent.getCPtr(e)) : tinyWRAPPINVOKE.SipCallback_OnPublicationChangedSwigExplicitSipCallback(swigCPtr, PublicationEvent.getCPtr(e)));
    return ret;
  }

  private void SwigDirectorConnect() {
    if (SwigDerivedClassHasMethod("OnRegistrationChanged", swigMethodTypes0))
      swigDelegate0 = new SwigDelegateSipCallback_0(SwigDirectorOnRegistrationChanged);
    if (SwigDerivedClassHasMethod("OnSubscriptionChanged", swigMethodTypes1))
      swigDelegate1 = new SwigDelegateSipCallback_1(SwigDirectorOnSubscriptionChanged);
    if (SwigDerivedClassHasMethod("OnPublicationChanged", swigMethodTypes2))
      swigDelegate2 = new SwigDelegateSipCallback_2(SwigDirectorOnPublicationChanged);
    tinyWRAPPINVOKE.SipCallback_director_connect(swigCPtr, swigDelegate0, swigDelegate1, swigDelegate2);
  }

  private bool SwigDerivedClassHasMethod(string methodName, Type[] methodTypes) {
    System.Reflection.MethodInfo methodInfo = this.GetType().GetMethod(methodName, System.Reflection.BindingFlags.Public | System.Reflection.BindingFlags.NonPublic | System.Reflection.BindingFlags.Instance, null, methodTypes, null);
    bool hasDerivedMethod = methodInfo.DeclaringType.IsSubclassOf(typeof(SipCallback));
    return hasDerivedMethod;
  }

  private int SwigDirectorOnRegistrationChanged(IntPtr e) {
    return OnRegistrationChanged((e == IntPtr.Zero) ? null : new RegistrationEvent(e, false));
  }

  private int SwigDirectorOnSubscriptionChanged(IntPtr e) {
    return OnSubscriptionChanged((e == IntPtr.Zero) ? null : new SubscriptionEvent(e, false));
  }

  private int SwigDirectorOnPublicationChanged(IntPtr e) {
    return OnPublicationChanged((e == IntPtr.Zero) ? null : new PublicationEvent(e, false));
  }

  public delegate int SwigDelegateSipCallback_0(IntPtr e);
  public delegate int SwigDelegateSipCallback_1(IntPtr e);
  public delegate int SwigDelegateSipCallback_2(IntPtr e);

  private SwigDelegateSipCallback_0 swigDelegate0;
  private SwigDelegateSipCallback_1 swigDelegate1;
  private SwigDelegateSipCallback_2 swigDelegate2;

  private static Type[] swigMethodTypes0 = new Type[] { typeof(RegistrationEvent) };
  private static Type[] swigMethodTypes1 = new Type[] { typeof(SubscriptionEvent) };
  private static Type[] swigMethodTypes2 = new Type[] { typeof(PublicationEvent) };
}