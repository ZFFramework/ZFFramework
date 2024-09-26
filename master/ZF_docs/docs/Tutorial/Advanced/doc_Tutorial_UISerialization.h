/**
 * @page DocTag_Tutorial_UISerialization UI serialization
 *
 * most of UI elements in ZFFramework is also serializable,
 * and it's easy to serialize from and to xml format\n
 * here's a quick example to see the xml output format:
 * @code
 *   zfobj<ZFUIView> parent;
 *   zfobj<ZFUIButtonBasic> child;
 *   parent->child(child)->c_sizeFill();
 *   child->label()->text("button");
 *
 *   ZFSerializableData data = ZFObjectToData(parent);
 *   zfstring xmlString;
 *   ZFSerializableDataToXml(ZFOutputForString(xmlString), data);
 *   ZFLogTrim() << "serialized data:";
 *   ZFLogTrim() << xmlString;
 *
 *   ZFSerializableData dataNew = ZFSerializableDataFromXml(ZFInputForString(xmlString));
 *   zfauto objNew = ZFObjectFromData(dataNew);
 *   ZFLogTrim() << "re-serialized object:";
 *   ZFLogTrim() << objNew;
 * @endcode
 * which would output these:
 * @code
 *   serialized data:
 *   <ZFUIView>
 *       <ZFUIButtonBasic category="child" >
 *           <ZFUITextView prop="labelNormal" >
 *               <zfstring prop="text" value="button" />
 *           </ZFUITextView>
 *           <ZFUILayoutParam category="layoutParam" >
 *               <ZFUISizeParam prop="sizeParam" value="(Fill, Fill)" />
 *           </ZFUILayoutParam>
 *       </ZFUIButtonBasic>
 *   </ZFUIView>
 *
 *   re-serialized object:
 *   <ZFUIView(0x7fe61e1340b0) (0, 0, 0, 0)>
 * @endcode
 */

